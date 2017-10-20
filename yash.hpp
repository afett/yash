/*
 * Copyright (c) 2014, 2015, 2017 Andreas Fett.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef YASH_HPP
#define YASH_HPP

#include <cassert>
#include <functional>
#include <algorithm>
#include <memory>
#include <vector>

// yet another signal library^W header

namespace yash {

namespace detail {

// private signal <-> connection interface
class callback_base {
public:
	virtual void disconnect() = 0;
	virtual ~callback_base() = default;
};

}

// handle for single slot connected to a signal
class connection {
public:
	connection() = default;
	~connection() { cb_.reset(); }

	// The connections share the actual callback closure if disconnect
	// is called on one of them, all are disconnected.
	connection(connection const& o) : cb_(o.cb_) { }
	connection(connection && o) : cb_(o.cb_) { o.cb_.reset(); }

	// Assignment to a connected connection will not disconnect it but
	// leave the callback connected to the signal. There is just no
	// external reference to it anymore.
	connection & operator=(connection const& o)
	{
		if (&o != this) {
			cb_ = o.cb_;
		}

		return *this;
	}

	connection & operator=(connection && o)
	{
		if (&o != this) {
			cb_ = o.cb_;
			o.cb_.reset();
		}
		return *this;
	}

	bool connected() const
	{ return !cb_.expired(); }

	void disconnect()
	{
		auto cb(cb_.lock());
		if (cb) {
			cb->disconnect();
			cb_.reset();
		}
	}

private:
	using callback_weak_ptr = std::weak_ptr<detail::callback_base>;
	using callback_ptr = std::shared_ptr<detail::callback_base>;

	template <typename T> friend class signal;
	connection(callback_ptr const& cb) : cb_(cb) { }

	callback_weak_ptr cb_;
};

// RAII wrapper for connection
class auto_connection {
public:
	auto_connection() = delete;
	auto_connection(auto_connection const&) = delete;
	auto_connection & operator=(auto_connection const&) = delete;

	auto_connection(connection const& conn)
	: conn_(conn) { }

	auto_connection(auto_connection && o)
	: conn_(std::move(o.conn_)) { }

	auto_connection & operator=(auto_connection && o)
	{
		conn_ = std::move(o.conn_);
		return *this;
	}

	void disconnect()
	{ conn_.disconnect(); }

	bool connected() const
	{ return conn_.connected(); }

	~auto_connection()
	{ conn_.disconnect(); }

private:
	connection conn_;
};

// Interface to expose a signal for a user without allowing to invoke it
template <typename T>
class signal_proxy {
public:
	using slot_type = std::function<T>;

	virtual connection connect(slot_type const&) = 0;
	virtual ~signal_proxy() = default;
};

template <typename T>
class signal : public signal_proxy<T> {
public:
	using proxy_type = signal_proxy<T>;
	using slot_type = typename signal_proxy<T>::slot_type;

	signal() = default;
	signal(signal && o) : cb_(std::move(o.cb_)) { }
	signal & operator=(signal && o)
	{
		cb_ = std::move(o.cb_);
		return *this;
	}

	// Rationale
	// The semantics of copying and assigning a signal are somewhat undefined.
	signal(signal const&) = delete;
	signal & operator=(signal const&) = delete;

	size_t slots() const
	{ return cb_.size(); }

	// The callback will not be called for any already active invokation
	// of this signal at the time connect() is called.
	connection connect(slot_type const& slot)
	{ return connection(add_callback(slot)); }

	template <typename... Args>
	void operator()(Args&&... args)
	{
		if (cb_.empty()) {
			return;
		}
		call_each({cb_.begin(), cb_.end()},
			std::forward<Args>(args)...);
	}

	template <typename... Args>
	void operator()(Args&&... args) const
	{
		if (cb_.empty()) {
			return;
		}
		call_each({cb_.begin(), cb_.end()},
			std::forward<Args>(args)...);
	}

private:
	struct callback : public detail::callback_base {
		callback(signal<T> *owner_, slot_type const& fn_)
		: owner(owner_), fn(fn_)
		{
			assert(owner && "no callback owner");
			assert(fn && "no callback function");
		}

		void disconnect()
		{
			if (owner) {
				owner->del_callback(this);
				owner = nullptr;
				fn = nullptr;
			}
		}

		signal<T> *owner;
		slot_type fn;
	};

	using callback_ptr = std::shared_ptr<callback>;
	using callback_weak_ptr = std::weak_ptr<callback>;

	callback_ptr add_callback(slot_type const& slot)
	{
		cb_.push_back(std::make_shared<callback>(this, slot));
		return cb_.back();
	}

	void del_callback(callback * cb)
	{
		assert(cb && "trying to remove null callback");

		auto it(std::find_if(cb_.begin(), cb_.end(),
			[cb](callback_ptr const& ptr){ return ptr.get() == cb; }));

		assert(it != cb_.end() && "trying to remove invalid callback");
		it->reset();
		cb_.erase(it);
	}

	template <typename... Args>
	static void call_each(std::vector<callback_weak_ptr> cbs, Args&&... args)
	{
		for (auto ptr: cbs) {
			auto cb(ptr.lock());
			if (cb) {
				cb->fn(std::forward<Args>(args)...);
			}
		}
	}

	std::vector<callback_ptr> cb_;
};

}

#endif
