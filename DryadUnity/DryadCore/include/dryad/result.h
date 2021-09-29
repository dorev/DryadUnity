#pragma once

#include "dryad/definitions.h"
#include "dryad/error.h"
#include "dryad/types.h"

namespace Dryad
{

static constexpr U64 Success = ErrorCode::NoError;

///////////////////////////////////////////////////////////////////////////////
// Result
///////////////////////////////////////////////////////////////////////////////

template <class ValueType = Empty>
class Result
{
public:

    Result()
        : _error()
        , _value()
    {
    }

    Result(const Result& other)
        : _error(other._value)
        , _value(other._error)
    {
    }

    Result(const ValueType& value)
        : _error(ErrorCode::NoError)
        , _value(value)
    {
    }

    Result(ValueType&& value)
        : _error(ErrorCode::NoError)
        , _value(value)
    {
    }

    Result(const Error& error)
        : _error(error)
        , _value()
    {
    }

    Result(Error&& error)
        : _error(error)
        , _value()
    {
    }

    virtual ~Result()
    {
    }

    bool isValid() const
    {
        return _error.getCode() == ErrorCode::NoError;
    }

    explicit operator bool() const
    {
        return isValid();
    }

    bool hasError()
    {
        return !isValid();
    }

    const Error& getError()
    {
        return _error;
    }

    const ValueType& getValueConst()
    {
        return _value;
    }

    ValueType& getValue()
    {
        return _value;
    }

protected:

    Error _error;
    ValueType _value;
};

enum class AsyncResultState
{
    Pending,
    Processing,
    Successful,
    Cancelled,
    Failed
};

///////////////////////////////////////////////////////////////////////////////
// AsyncResult
///////////////////////////////////////////////////////////////////////////////

template <class ValueType>
class AsyncResult : public Result<ValueType>
{
    using Base = Result<ValueType>;

public:

    AsyncResult()
        : _locked(false)
        , _state(AsyncResultState::Pending)
    {
    }

    AsyncResult(const AsyncResult& other)
        : Result<ValueType>(other)
        , _locked(other._locked)
        , _state(other._state)
    {
    }

    bool isReady() const
    {
        return _state != AsyncResultState::Pending
            && _state != AsyncResultState::Processing;
    }

    bool isPending() const
    {
        return _state == AsyncResultState::Pending;
    }

    bool isProcessing() const
    {
        return _state == AsyncResultState::Processing;
    }

    bool isSuccessful() const
    {
        return _state == AsyncResultState::Successful;
    }

    bool isCancelled() const
    {
        return _state == AsyncResultState::Cancelled;
    }

    bool isFailed() const
    {
        return _state == AsyncResultState::Failed;
    }

    const AsyncResultState& getState() const
    {
        return _state;
    }

private:

    // Using the AsyncResultUpdater is a way to prevent the user of the AsyncResult
    // from directly (or discretly) update its content and/or state while keeping
    // a simple way to do so
    class AsyncResultUpdater;
    friend void AsyncResultUpdater::startProcessing(AsyncResult<ValueType>&);
    friend Error AsyncResultUpdater::update(AsyncResult<ValueType>&, const ValueType&, AsyncResultState, bool);
    friend Error AsyncResultUpdater::update(AsyncResult<ValueType>&, ValueType&&, AsyncResultState, bool);
    friend Error AsyncResultUpdater::update(AsyncResult<ValueType>&, Error&&, AsyncResultState, bool);

    void startProcessing()
    {
        _state = AsyncResultState::Processing;
    }

    Error update(const ValueType& value, AsyncResultState state, bool lock = false)
    {
        if(_locked)
            return Error{ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};

        Base::_value = value;
        _state = state;
        return Error{0};
    }

    Error update(ValueType&& value, AsyncResultState state, bool lock = false)
    {
        if(_locked)
            return Error{ ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};

        Base::_value = std::move(value);
        Base::_state = state;
        _locked = lock;
        return Error{0};
    }

    Error update(Error&& error, AsyncResultState state, bool lock = false)
    {
        if(_locked)
            return Error{ ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};

        Base::_error = std::move(error);
        _state = state;
        _locked = lock;
        return Error{0};
    }

    bool _locked;
    AsyncResultState _state;
};


class AsyncResultUpdater
{
public:

    AsyncResultUpdater() = delete;
    DELETE_COPY_CONSTRUCTOR(AsyncResultUpdater);

    template <class ValueType>
    static void startProcessing(AsyncResult<ValueType>& asyncResult)
    {
        asyncResult.startProcessing();
    }

    template <class ValueType>
    static Error update(AsyncResult<ValueType>& asyncResult, const ValueType& value, AsyncResultState state, bool lock = false)
    {
        return asyncResult.update(value, state, lock);
    }

    template <class ValueType>
    static Error update(AsyncResult<ValueType>& asyncResult, ValueType&& value, AsyncResultState state, bool lock = false)
    {
        return asyncResult.update(std::forward<ValueType>(value), state, lock);
    }

    template <class ValueType>
    static Error update(AsyncResult<ValueType>& asyncResult, Error&& error, AsyncResultState state, bool lock = false)
    {
        return asyncResult.update(std::forward<Error>(error), state, lock);
    }
};

} // namespace Utils
