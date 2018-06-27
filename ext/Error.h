#pragma once

std::string WError(LPTSTR lpszFunction);
void JSError(LPTSTR lpszFunction);

class WinException : public std::exception
{
public:
  WinException(const std::string message)
  {
    msg = WError(TEXT((LPTSTR)message.c_str()));
  }
  const char *what() const throw()
  {
    return msg.c_str();
  }

protected:
  std::string msg;
};
