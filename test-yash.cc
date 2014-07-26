#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "yash.hpp"

#define TEST_ASSERT(expr) \
	assert(expr);

#define RUN_TEST(test)                       \
{                                            \
	std::cerr << "running " # test "\n"; \
	test();                              \
}

namespace tr1 = std::tr1;

struct nil {};

template <typename A0 = nil,
	  typename A1 = nil,
	  typename A2 = nil,
	  typename A3 = nil,
	  typename A4 = nil,
	  typename A5 = nil,
	  typename A6 = nil,
	  typename A7 = nil>
struct call_result {
	call_result()
	: called(false), a0(), a1(), a2(), a3(), a4(), a5(), a6(), a7()
	{ }

	bool called;
	A0 a0;
	A1 a1;
	A2 a2;
	A3 a3;
	A4 a4;
	A5 a5;
	A6 a6;
	A7 a7;
};

void f0(call_result<> & res)
{
	res.called = true;
}

template <typename A0>
void f(call_result<A0> & res, A0 a0)
{
	res.called = true;
	res.a0 = a0;
}

template <typename A0, typename A1>
void f(call_result<A0, A1> & res, A0 a0, A1 a1)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
}

template <typename A0, typename A1, typename A2>
void f(call_result<A0, A1, A2> & res, A0 a0, A1 a1, A2 a2)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
}

template <typename A0, typename A1, typename A2, typename A3>
void f(call_result<A0, A1, A2, A3> & res, A0 a0, A1 a1, A2 a2, A3 a3)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
	res.a3 = a3;
}

template <typename A0, typename A1, typename A2, typename A3,
          typename A4>
void f(call_result<A0, A1, A2, A3, A4> & res, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
	res.a3 = a3;
	res.a4 = a4;
}

template <typename A0, typename A1, typename A2, typename A3,
          typename A4, typename A5>
void f(call_result<A0, A1, A2, A3, A4, A5> & res,
       A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
	res.a3 = a3;
	res.a4 = a4;
	res.a5 = a5;
}

template <typename A0, typename A1, typename A2, typename A3,
          typename A4, typename A5, typename A6>
void f(call_result<A0, A1, A2, A3, A4, A5, A6> & res,
       A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
	res.a3 = a3;
	res.a4 = a4;
	res.a5 = a5;
	res.a6 = a6;
}

template <typename A0, typename A1, typename A2, typename A3,
          typename A4, typename A5, typename A6, typename A7>
void f(call_result<A0, A1, A2, A3, A4, A5, A6, A7> & res,
       A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
	res.called = true;
	res.a0 = a0;
	res.a1 = a1;
	res.a2 = a2;
	res.a3 = a3;
	res.a4 = a4;
	res.a5 = a5;
	res.a6 = a6;
	res.a7 = a7;
}

class A {
public:
	A()
	:
		foo_called_(false),
		foo_arg_(),
		bar_called_(false)
	{ }

	void foo(std::string const& arg)
	{
		foo_called_ = true;
		foo_arg_ = arg;
	}

	void bar() const
	{
		bar_called_ = true;
	}

	bool foo_called_;
	std::string foo_arg_;
	mutable bool bar_called_;
};

void test_arg0()
{
	call_result<> res;
	yash::signal<void(void)> sig;
	sig.connect(tr1::bind(
		&f0, tr1::ref(res)));
	sig();
	TEST_ASSERT(res.called);
}

void test_arg1()
{
	call_result<int> res;
	yash::signal<void(int)> sig;
	sig.connect(tr1::bind(
		&f<int>, tr1::ref(res),
		tr1::placeholders::_1));
	sig(1);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
}

void test_arg2()
{
	call_result<int, int> res;
	yash::signal<void(int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2));
	sig(1, 2);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
}

void test_arg3()
{
	call_result<int, int, int> res;
	yash::signal<void(int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3));
	sig(1, 2, 3);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
}

void test_arg4()
{
	call_result<int, int, int, int> res;
	yash::signal<void(int, int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3,
		tr1::placeholders::_4));
	sig(1, 2, 3, 4);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
	TEST_ASSERT(res.a3 == 4);
}

void test_arg5()
{
	call_result<int, int, int, int, int> res;
	yash::signal<void(int, int, int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3,
		tr1::placeholders::_4,
		tr1::placeholders::_5));
	sig(1, 2, 3, 4, 5);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
	TEST_ASSERT(res.a3 == 4);
	TEST_ASSERT(res.a4 == 5);
}

void test_arg6()
{
	call_result<int, int, int, int, int, int> res;
	yash::signal<void(int, int, int, int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int, int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3,
		tr1::placeholders::_4,
		tr1::placeholders::_5,
		tr1::placeholders::_6));
	sig(1, 2, 3, 4, 5, 6);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
	TEST_ASSERT(res.a3 == 4);
	TEST_ASSERT(res.a4 == 5);
	TEST_ASSERT(res.a5 == 6);
}

void test_arg7()
{
	call_result<int, int, int, int, int, int, int> res;
	yash::signal<void(int, int, int, int, int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int, int, int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3,
		tr1::placeholders::_4,
		tr1::placeholders::_5,
		tr1::placeholders::_6,
		tr1::placeholders::_7));
	sig(1, 2, 3, 4, 5, 6, 7);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
	TEST_ASSERT(res.a3 == 4);
	TEST_ASSERT(res.a4 == 5);
	TEST_ASSERT(res.a5 == 6);
	TEST_ASSERT(res.a6 == 7);
}

void test_arg8()
{
	call_result<int, int, int, int, int, int, int, int> res;
	yash::signal<void(int, int, int, int, int, int, int, int)> sig;
	sig.connect(tr1::bind(
		&f<int, int, int, int, int, int, int, int>, tr1::ref(res),
		tr1::placeholders::_1,
		tr1::placeholders::_2,
		tr1::placeholders::_3,
		tr1::placeholders::_4,
		tr1::placeholders::_5,
		tr1::placeholders::_6,
		tr1::placeholders::_7,
		tr1::placeholders::_8));
	sig(1, 2, 3, 4, 5, 6, 7, 8);
	TEST_ASSERT(res.called);
	TEST_ASSERT(res.a0 == 1);
	TEST_ASSERT(res.a1 == 2);
	TEST_ASSERT(res.a2 == 3);
	TEST_ASSERT(res.a3 == 4);
	TEST_ASSERT(res.a4 == 5);
	TEST_ASSERT(res.a5 == 6);
	TEST_ASSERT(res.a6 == 7);
	TEST_ASSERT(res.a7 == 8);
}

void test_memfun()
{
	A a;
	yash::signal<void(std::string)> sig;
	sig.connect(tr1::bind(&A::foo,
		&a, tr1::placeholders::_1));
	sig("hello");
	TEST_ASSERT(a.foo_called_);
	TEST_ASSERT(a.foo_arg_ == "hello");
}

void test_const_memfun()
{
	A a;
	yash::signal<void(void)> sig;
	sig.connect(tr1::bind(&A::bar, tr1::ref(a)));
	sig();
	TEST_ASSERT(a.bar_called_);
}

class B {
public:
	typedef yash::signal<void(void)> signal_type;

	signal_type::proxy_type & sig()
	{
		return sig_;
	}

	signal_type::proxy_type & sigr;

	B() : sigr(sig_) {}

	void run()
	{
		sig_();
	}

private:
	signal_type sig_;
};

void test_signal_proxy()
{
	B b;

	call_result<> res1;
	call_result<> res2;
	b.sig().connect(tr1::bind(&f0, tr1::ref(res1)));
	b.sigr.connect(tr1::bind(&f0, tr1::ref(res2)));
	b.run();

	TEST_ASSERT(res1.called);
	TEST_ASSERT(res2.called);
}

void count_call(size_t & called, size_t index)
{
	TEST_ASSERT(called == index);
	++called;
}

void test_call_order()
{
	yash::signal<void(void)> sig;
	size_t called(0);
	sig.connect(tr1::bind(
		&count_call, tr1::ref(called), 0));
	sig.connect(tr1::bind(
		&count_call, tr1::ref(called), 1));
	sig.connect(tr1::bind(
		&count_call, tr1::ref(called), 2));
	sig.connect(tr1::bind(
		&count_call, tr1::ref(called), 3));
	sig.connect(tr1::bind(
		&count_call, tr1::ref(called), 4));
	TEST_ASSERT(sig.slots() == 5);
	sig();
	TEST_ASSERT(called == 5);
}

void test_connection_disconnect()
{
	call_result<> res1;
	call_result<> res2;
	yash::signal<void(void)> sig;
	yash::connection conn1(sig.connect(tr1::bind(
		&f0, tr1::ref(res1))));
	TEST_ASSERT(conn1.connected());
	TEST_ASSERT(sig.slots() == 1);

	yash::connection conn2(sig.connect(tr1::bind(
		&f0, tr1::ref(res2))));
	TEST_ASSERT(conn2.connected());
	TEST_ASSERT(sig.slots() == 2);

	sig();
	TEST_ASSERT(res1.called);
	TEST_ASSERT(res2.called);

	conn1.disconnect();
	TEST_ASSERT(!conn1.connected());
	TEST_ASSERT(conn2.connected());
	TEST_ASSERT(sig.slots() == 1);

	res1.called = false;
	res2.called = false;
	sig();
	TEST_ASSERT(!res1.called);
	TEST_ASSERT(res2.called);
}

void test_connection()
{
	call_result<> res;
	yash::signal<void(void)> sig;

	yash::connection conn;
	TEST_ASSERT(!conn.connected());
	conn.disconnect();
	TEST_ASSERT(!conn.connected());
	TEST_ASSERT(sig.slots() == 0);

	yash::connection conn1(sig.connect(tr1::bind(
		&f0, tr1::ref(res))));
	TEST_ASSERT(sig.slots() == 1);
	TEST_ASSERT(conn1.connected());

	conn = conn1;
	TEST_ASSERT(conn.connected());

	yash::connection conn2(conn);
	TEST_ASSERT(conn2.connected());

	TEST_ASSERT(sig.slots() == 1);
	conn2.disconnect();
	TEST_ASSERT(sig.slots() == 0);
	TEST_ASSERT(!conn.connected());
	TEST_ASSERT(!conn1.connected());
	TEST_ASSERT(!conn2.connected());
}

void disconnect(bool & called, yash::connection & conn)
{
	called = true;
	conn.disconnect();
	TEST_ASSERT(!conn.connected());
}

void test_connection_disconnect_self()
{
	bool called(false);
	yash::signal<void(void)> sig;
	yash::connection conn;
	yash::connection conn1(sig.connect(tr1::bind(
		&disconnect, tr1::ref(called), tr1::ref(conn))));
	TEST_ASSERT(sig.slots() == 1);

	conn = conn1;
	TEST_ASSERT(conn.connected());
	TEST_ASSERT(conn1.connected());

	sig();
	TEST_ASSERT(called);
	TEST_ASSERT(!conn.connected());
	TEST_ASSERT(!conn1.connected());
	TEST_ASSERT(sig.slots() == 0);

	called = false;
	sig();
	TEST_ASSERT(!called);
}

void test_connection_disconnect_next()
{
	bool called(false);
	yash::signal<void(void)> sig;
	yash::connection conn;
	yash::connection conn1(sig.connect(tr1::bind(
		&disconnect, tr1::ref(called), tr1::ref(conn))));

	call_result<> res;
	yash::connection conn2(sig.connect(tr1::bind(
		&f0, tr1::ref(res))));
	TEST_ASSERT(sig.slots() == 2);

	conn = conn2;
	TEST_ASSERT(conn.connected());
	TEST_ASSERT(conn1.connected());
	TEST_ASSERT(conn2.connected());

	sig();
	TEST_ASSERT(sig.slots() == 1);
	TEST_ASSERT(called);
	TEST_ASSERT(!res.called);
	TEST_ASSERT(conn1.connected());
	TEST_ASSERT(!conn.connected());
	TEST_ASSERT(!conn2.connected());

	called = false;
	res.called = false;
	sig();
	TEST_ASSERT(called);
	TEST_ASSERT(!res.called);
}

void test_connection_disconnect_prev()
{
	bool called(false);
	yash::signal<void(void)> sig;
	yash::connection conn;

	call_result<> res;
	yash::connection conn1(sig.connect(tr1::bind(
		&f0, tr1::ref(res))));
	yash::connection conn2(sig.connect(tr1::bind(
		&disconnect, tr1::ref(called), tr1::ref(conn))));
	TEST_ASSERT(sig.slots() == 2);

	conn = conn1;
	TEST_ASSERT(conn.connected());
	TEST_ASSERT(conn1.connected());
	TEST_ASSERT(conn2.connected());

	sig();
	TEST_ASSERT(sig.slots() == 1);
	TEST_ASSERT(called);
	TEST_ASSERT(res.called);
	TEST_ASSERT(!conn1.connected());
	TEST_ASSERT(!conn.connected());
	TEST_ASSERT(conn2.connected());

	called = false;
	res.called = false;
	sig();
	TEST_ASSERT(called);
	TEST_ASSERT(!res.called);
}

class E {
public:
	E(yash::signal<void(void)> & sig)
	:
		called(0)
	{
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::disconnect_all, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
		conn_.push_back(sig.connect(tr1::bind(&E::foo, this)));
	}

	void foo()
	{
		++called;
	}

	void disconnect_all()
	{
		++called;
		for (size_t i(0); i < conn_.size(); ++i) {
			TEST_ASSERT(conn_[i].connected());
			conn_[i].disconnect();
			TEST_ASSERT(!conn_[i].connected());
		}
	}

	std::vector<yash::connection> conn_;
	size_t called;
};

void test_connection_disconnect_all()
{
	yash::signal<void(void)> sig;
	E e(sig);

	TEST_ASSERT(sig.slots() == 8);
	sig();
	TEST_ASSERT(sig.slots() == 0);
	TEST_ASSERT(e.called == 5);

	e.called = 0;
	sig();
	TEST_ASSERT(e.called == 0);
}

class C {
public:
	typedef yash::signal<void(void)> signal_type;

	signal_type::proxy_type & sig;

	C() : sig(sig_) {}

	void run() const
	{
		sig_();
	}

private:
	signal_type sig_;
};

void test_const_signal()
{
	C c;

	call_result<> res;
	c.sig.connect(tr1::bind(&f0, tr1::ref(res)));
	c.run();

	TEST_ASSERT(res.called);
}

void connect(size_t & called, yash::signal<void(void)> & sig)
{
	++called;
	sig.connect(tr1::bind(&connect, tr1::ref(called), tr1::ref(sig)));
}

void test_connect_in_callback()
{
	size_t called(0);
	yash::signal<void(void)> sig;

	sig.connect(tr1::bind(&connect, tr1::ref(called), tr1::ref(sig)));
	TEST_ASSERT(sig.slots() == 1);
	sig();
	TEST_ASSERT(called == 1);
	TEST_ASSERT(sig.slots() == 2);
	called = 0;

	sig();
	TEST_ASSERT(called == 2);
	TEST_ASSERT(sig.slots() == 4);
}

void test_auto_connection()
{
	call_result<> res1;
	call_result<> res2;
	call_result<> res3;
	yash::signal<void(void)> sig;

	yash::auto_connection conn1(sig.connect(tr1::bind(
		&f0, tr1::ref(res1))));
	TEST_ASSERT(conn1.connected());
	{
		yash::auto_connection conn2(sig.connect(tr1::bind(
			&f0, tr1::ref(res2))));
		TEST_ASSERT(conn2.connected());
		TEST_ASSERT(sig.slots() == 2);

		yash::auto_connection conn3(sig.connect(tr1::bind(
			&f0, tr1::ref(res3))));
		TEST_ASSERT(conn3.connected());
		TEST_ASSERT(sig.slots() == 3);

		sig();

		TEST_ASSERT(res1.called);
		TEST_ASSERT(res2.called);
		TEST_ASSERT(res3.called);
		res1.called = false;
		res2.called = false;
		res3.called = false;

		conn3.disconnect();
		TEST_ASSERT(!conn3.connected());
		TEST_ASSERT(sig.slots() == 2);

		sig();

		TEST_ASSERT(res1.called);
		TEST_ASSERT(res2.called);
		TEST_ASSERT(!res3.called);
		res1.called = false;
		res2.called = false;
	}

	TEST_ASSERT(sig.slots() == 1);
	TEST_ASSERT(conn1.connected());

	sig();
	TEST_ASSERT(res1.called);
	TEST_ASSERT(!res2.called);
	TEST_ASSERT(!res3.called);
}

struct functor0 {
	functor0()
	:
		called_(false)
	{ }

	void operator()()
	{
		called_ = true;
	}

	bool called_;
};

void test_functor_0arg()
{
	yash::signal<void(void)> sig;

	functor0 f0;

	// this will create a copy of f0!
	sig.connect(f0);
	sig();
	TEST_ASSERT(!f0.called_);

	// this will not
	sig.connect(std::tr1::ref(f0));
	sig();

	TEST_ASSERT(f0.called_);
}

void test_signal_deconstruct()
{
	call_result<> res;
	yash::connection conn;
	TEST_ASSERT(!conn.connected());
	{
		yash::signal<void(void)> sig;
		conn = sig.connect(tr1::bind(&f0, tr1::ref(res)));
		TEST_ASSERT(conn.connected());
	}
	TEST_ASSERT(!conn.connected());
}

void test_connection_assign()
{
	call_result<> res1;
	yash::signal<void(void)> sig1;
	yash::connection conn1(sig1.connect(tr1::bind(&f0, tr1::ref(res1))));
	TEST_ASSERT(conn1.connected());
	TEST_ASSERT(sig1.slots() == 1);

	call_result<> res2;
	yash::signal<void(void)> sig2;
	yash::connection conn2(sig2.connect(tr1::bind(&f0, tr1::ref(res2))));
	TEST_ASSERT(conn2.connected());
	TEST_ASSERT(sig2.slots() == 1);

	conn2 = conn1;
	TEST_ASSERT(sig1.slots() == 1);
	TEST_ASSERT(sig2.slots() == 1);

	conn2.disconnect();
	TEST_ASSERT(sig1.slots() == 0);
	TEST_ASSERT(sig2.slots() == 1);

	sig1();
	TEST_ASSERT(!res1.called);

	sig2();
	TEST_ASSERT(res2.called);
}

int main()
{
	RUN_TEST(test_arg0);
	RUN_TEST(test_arg1);
	RUN_TEST(test_arg2);
	RUN_TEST(test_arg3);
	RUN_TEST(test_arg4);
	RUN_TEST(test_arg5);
	RUN_TEST(test_arg6);
	RUN_TEST(test_arg7);
	RUN_TEST(test_arg8);
	RUN_TEST(test_memfun);
	RUN_TEST(test_const_memfun);
	RUN_TEST(test_signal_proxy);
	RUN_TEST(test_call_order);
	RUN_TEST(test_connection);
	RUN_TEST(test_connection_disconnect);
	RUN_TEST(test_connection_disconnect_self);
	RUN_TEST(test_connection_disconnect_next);
	RUN_TEST(test_connection_disconnect_prev);
	RUN_TEST(test_connection_disconnect_all);
	RUN_TEST(test_const_signal);
	RUN_TEST(test_connect_in_callback);
	RUN_TEST(test_auto_connection);
	RUN_TEST(test_functor_0arg);
	RUN_TEST(test_signal_deconstruct);
	RUN_TEST(test_connection_assign);
	return 0;
}
