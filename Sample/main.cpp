#include <QCoreApplication>
#include <QDateTime>
#include <qexceptionstream.h>

class TestExc : public std::exception
{
public:
	TestExc(const QString &msg) :
		_msg{msg.toUtf8()}
	{}

	const char *what() const noexcept override {
		return _msg.constData();
	}

private:
	const QByteArray _msg;
};

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	try {
		qThrow(TestExc) << "Hello you little" << 42
						<< "- it is currently" << QDateTime::currentDateTime();
	} catch(TestExc &e) {
		qDebug() << "CAUGHT:" << e.what();
	}

	return 0;
}
