// RUN: %clang_cc1 "-triple" "x86_64-apple-darwin9.0.0" -fsyntax-only -verify %s

void f0(int) __attribute__((availability(macosx,introduced=10.4,deprecated=10.6)));
void f1(int) __attribute__((availability(macosx,introduced=10.5)));
void f2(int) __attribute__((availability(macosx,introduced=10.4,deprecated=10.5)));
void f3(int) __attribute__((availability(macosx,introduced=10.6)));
void f4(int) __attribute__((availability(macosx,introduced=10.1,deprecated=10.3,obsoleted=10.5), availability(ios,introduced=2.0,deprecated=3.0))); // expected-note{{explicitly marked unavailable}}

void test() {
  f0(0);
  f1(0);
  f2(0); // expected-warning{{'f2' is deprecated: first deprecated in Mac OS X 10.5}}
  f3(0);
  f4(0); // expected-error{{f4' is unavailable: obsoleted in Mac OS X 10.5}}
}