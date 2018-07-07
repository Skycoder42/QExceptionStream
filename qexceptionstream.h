#ifndef QEXCEPTIONSTREAM_H
#define QEXCEPTIONSTREAM_H

#include <exception>
#include <type_traits>

#include <QDebug>

template <typename TException>
class QExceptionStream : public QDebug
{
	static_assert(std::is_base_of<std::exception, TException>::value, "TException must implement std::exception");
	Q_DISABLE_COPY(QExceptionStream)

public:
	QExceptionStream();
	~QExceptionStream() noexcept(false);

	QExceptionStream<TException> &allowThrow();
	QExceptionStream<TException> &disallowThrow();

private:
	QString _msg;
	bool _canThrow = true;
};

#define qThrow(type) QExceptionStream<type>{}



template<typename TException>
QExceptionStream<TException>::QExceptionStream() :
	QDebug{QtDebugMsg}
{
	QDebug tmpDbg{&_msg};
	QDebug::swap(tmpDbg); //because of initialization order
}

template<typename TException>
QExceptionStream<TException>::~QExceptionStream() noexcept(false)
{
	{
		QDebug tmpDbg{QtDebugMsg};
		QDebug::swap(tmpDbg); // force flushing by "destroying" the internal debug early
	}
	if(_canThrow)
		throw TException{std::move(_msg)};
}

template<typename TException>
QExceptionStream<TException> &QExceptionStream<TException>::allowThrow()
{
	_canThrow = true;
	return *this;
}

template<typename TException>
QExceptionStream<TException> &QExceptionStream<TException>::disallowThrow()
{
	_canThrow = false;
	return *this;
}


#endif // QEXCEPTIONSTREAM_H
