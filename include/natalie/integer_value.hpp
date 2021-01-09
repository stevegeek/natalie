#pragma once

#include <assert.h>

#include "natalie/class_value.hpp"
#include "natalie/forward.hpp"
#include "natalie/global_env.hpp"
#include "natalie/macros.hpp"
#include "natalie/types.hpp"
#include "natalie/value.hpp"

namespace Natalie {

struct IntegerValue : Value {
    IntegerValue(Env *env, nat_int_t integer)
        : Value { Value::Type::Integer, env->Integer() }
        , m_integer { integer } { }

    nat_int_t to_nat_int_t() {
        return m_integer;
    }

    bool is_zero() {
        return m_integer == 0;
    }

    bool is_odd() {
        return m_integer % 2 != 0;
    }

    bool is_even() {
        return m_integer % 2 == 0;
    }

    static IntegerValue *from_size_t(Env *env, size_t number) {
        assert(number <= NAT_INT_MAX);
        return new IntegerValue { env, static_cast<nat_int_t>(number) };
    }

    Value *inspect(Env *env) { return to_s(env); }

    Value *to_s(Env *, Value * = nullptr);
    Value *to_i();
    Value *add(Env *, Value *);
    Value *sub(Env *, Value *);
    Value *mul(Env *, Value *);
    Value *div(Env *, Value *);
    Value *mod(Env *, Value *);
    Value *pow(Env *, Value *);
    Value *cmp(Env *, Value *);
    Value *eqeqeq(Env *, Value *);
    Value *times(Env *, Block *);
    Value *bitwise_and(Env *, Value *);
    Value *bitwise_or(Env *, Value *);
    Value *succ(Env *);
    Value *coerce(Env *, Value *);
    Value *abs(Env *);
    Value *chr(Env *);

    bool eq(Env *, Value *);
    bool eql(Env *, Value *);
    bool lt(Env *, Value *);
    bool lte(Env *, Value *);
    bool gt(Env *, Value *);
    bool gte(Env *, Value *);

private:
    nat_int_t m_integer { 0 };
};

}
