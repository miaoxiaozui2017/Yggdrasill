#include "SerialWindowsImpl.hpp"

#ifdef _WIN32

bool SerialImpl::Open(const SerialConfig& conf)
{
	m_hCom = CreateFile(conf.serialName.c_str(),
		GENERIC_READ | GENERIC_WRITE, // read & write
		0,
		NULL,
		OPEN_EXISTING,
		conf.isBlocked ? 0 : FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		perror("Serial can not open");
		return false;
	}
	return Config(conf);
}

bool SerialImpl::Close()
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(m_hCom))
		{
			perror("Close failed");
			return false;
		}
		m_hCom = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool SerialImpl::Config(const SerialConfig & conf)
{
	// buffer
	if (!SetupComm(m_hCom, 4096, 4096))
	{
		perror("Can not setup Com");
		return false;
	}
	// timeout
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 500;
	timeouts.ReadTotalTimeoutConstant = 5000;
	timeouts.WriteTotalTimeoutMultiplier = 500;
	timeouts.WriteTotalTimeoutConstant = 2000;
	if (!SetCommTimeouts(m_hCom, &timeouts))
	{
		perror("Can not setup timeout");
		return false;
	}

	DCB dcb;
	if (!GetCommState(m_hCom, &dcb))
	{
		perror("Can not get conf");
		return false;
	}
	dcb.BaudRate = conf.speed;
	dcb.fParity = 1;
	dcb.Parity = 0;
	if (conf.databits >= 4 && conf.databits <= 8)
	{
		dcb.ByteSize = conf.databits;
	}
	else
	{
		fprintf(stderr, "Unsupported databits.\n");
		return false;
	}
	
	switch (conf.parity)
	{
	case 'n':
	case 'N':
		dcb.Parity = NOPARITY;
		break;
	case 'o':
	case 'O':
		dcb.Parity = ODDPARITY;
		break;
	case 'e':
	case 'E':
		dcb.Parity = EVENPARITY;
		break;
	case 's':
	case 'S':
	default:
		fprintf(stderr, "Unspported parity.\n");
		return false;
	}

	switch (conf.stopbits)
	{
	case 1:
		dcb.StopBits = ONESTOPBIT;
		break;
	case 2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	case 3:
		dcb.StopBits = ONE5STOPBITS;
		break;
	default:
		fprintf(stderr, "Unspported stopbits.\n");
		return false;
	}

	if (!SetCommState(m_hCom, &dcb))
	{
		perror("Can not set config");
		return false;
	}

	if (!PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR))
	{
		perror("Can not purge Comm");
		return false;
	}

	return false;
}

#endif