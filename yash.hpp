/*
 * Copyright (c) 2014 Andreas Fett.
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
#include <tr1/functional>
#include <tr1/memory>
#include <vector>

// yet another signal library^W header

namespace yash {

namespace detail {

// private signal <-> connection interface
class callback_base {
public:
	virtual void disconnect() = 0;
	virtual ~callback_base() {}
};

}

// handle for single slot connected to a signal
class connection {
public:
	connection() : cb_() { }
	~connection() { cb_.reset(); }

	// the connections share the actual callback closure
	// if disconnect is called on one of them, all are
	// disconnected
	connection(connection const& o)
	{ cb_ = o.cb_; }

	connection & operator=(connection const& o)
	{
		if (&o != this) {
			cb_ = o.cb_;
		}

		return *this;
	}

	bool connected() const
	{ return !cb_.expired(); }

	void disconnect()
	{
		callback_ptr cb(cb_.lock());
		if (cb) {
			cb->disconnect();
			cb_.reset();
		}
	}

private:
	typedef std::tr1::weak_ptr<detail::callback_base> callback_weak_ptr;
	typedef std::tr1::shared_ptr<detail::callback_base> callback_ptr;

	template <typename T> friend class signal;
	connection(callback_ptr const& cb) : cb_(cb) { }

	callback_weak_ptr cb_;
};

// RAII wrapper for connection
class auto_connection {
public:
	auto_connection(connection const& conn)
	: conn_(conn) { }

	void disconnect()
	{ conn_.disconnect(); }

	bool connected() const
	{ return conn_.connected(); }

	~auto_connection()
	{ conn_.disconnect(); }

private:
	auto_connection(); // = deleted
	auto_connection(auto_connection const&); // = deleted
	auto_connection & operator=(auto_connection const&); // = deleted

	connection conn_;
};

// interface to expose a signal for a user without
// allowing to invoke it.
template <typename T>
class signal_proxy {
public:
	typedef std::tr1::function<T> slot_type;

	virtual connection connect(slot_type const&) = 0;
	virtual ~signal_proxy() {}
};

template <typename T>
class signal : public signal_proxy<T> {
public:
	typedef signal_proxy<T> proxy_type;
	typedef typename signal_proxy<T>::slot_type slot_type;

	signal() : cb_() { }
	~signal() {}

	size_t slots() const
	{ return cb_.size(); }

	// the callback will not be called for any already active
	// invokation of this signal at the time connect() is called.
	connection connect(slot_type const& slot)
	{ return connection(add_callback(slot)); }

	#define YASH_SIGNAL_CALL(...)                                        \
	{                                                               \
		typedef std::vector<callback_weak_ptr> container;       \
		typedef typename container::const_iterator iterator;    \
		container tmp(cb_.begin(), cb_.end());                  \
		for (iterator it(tmp.begin()); it != tmp.end(); ++it) { \
			callback_ptr cb(it->lock());                    \
			if (cb) {                                       \
				cb->fn(__VA_ARGS__);                    \
			}                                               \
		}                                                       \
	}

	void operator()()
	{ YASH_SIGNAL_CALL(); }

	void operator()() const
	{ YASH_SIGNAL_CALL(); }

	template <typename A0>
	void operator()(A0 a0)
	{ YASH_SIGNAL_CALL(a0); }

	template <typename A0>
	void operator()(A0 a0) const
	{ YASH_SIGNAL_CALL(a0); }

	template <typename A0, typename A1>
	void operator()(A0 a0, A1 a1)
	{ YASH_SIGNAL_CALL(a0, a1); }

	template <typename A0, typename A1>
	void operator()(A0 a0, A1 a1) const
	{ YASH_SIGNAL_CALL(a0, a1); }

	template <typename A0, typename A1, typename A2>
	void operator()(A0 a0, A1 a1, A2 a2)
	{ YASH_SIGNAL_CALL(a0, a1, a2); }

	template <typename A0, typename A1, typename A2>
	void operator()(A0 a0, A1 a1, A2 a2) const
	{ YASH_SIGNAL_CALL(a0, a1, a2); }

	template <typename A0, typename A1, typename A2, typename A3>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3)
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3); }

	template <typename A0, typename A1, typename A2, typename A3>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3) const
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5, typename A6>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5, a6); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5, typename A6>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5, a6); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5, typename A6, typename A7>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5, a6, a7); }

	template <typename A0, typename A1, typename A2, typename A3,
		  typename A4, typename A5, typename A6, typename A7>
	void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
	{ YASH_SIGNAL_CALL(a0, a1, a2, a3, a4, a5, a6, a7); }

	#undef YASH_SIGNAL_CALL

private:
	signal(signal const&); // = deleted
	signal & operator=(signal const&); // = deleted

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
				owner = 0;
				fn = 0;
			}
		}

		signal<T> *owner;
		slot_type fn;
	};

	typedef std::tr1::shared_ptr<callback> callback_ptr;
	typedef std::tr1::weak_ptr<callback> callback_weak_ptr;

	callback_ptr add_callback(slot_type const& slot)
	{
		callback_ptr cb(new callback(this, slot));
		cb_.push_back(cb);
		return cb;
	}

	void del_callback(callback * cb)
	{
		assert(cb && "trying to remove null callback");

		typedef std::vector<callback_ptr> container;
		typedef typename container::iterator iterator;

		for (iterator it(cb_.begin()); it != cb_.end(); ++it) {
			if (it->get() == cb) {
				it->reset();
				cb_.erase(it);
				return;
			}
		}

		assert(false && "trying to remove invalid callback");
	}

	std::vector<callback_ptr> cb_;
};

}

#endif
