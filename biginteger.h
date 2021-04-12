#include <iostream>
#include <vector>
#include <string>

const long long base = 1000000000;


class BigInteger {
public:
    BigInteger() : nums_({0}), is_negative_(false) {}

    BigInteger(int x) {
      if (x < 0) {
        is_negative_ = true;
        x = -x;
      } else {
        is_negative_ = false;
      }
      while (x > 0) {
        nums_.push_back(x % base);
        x /= base;
      }
      if (nums_.size() == 0) {
        nums_.push_back(0);
      }
      Balance(*this);
    }


    void Balance(BigInteger &x) {
      while (x.nums_.size() > 1 && x.nums_.back() == 0) {
        x.nums_.pop_back();
      }
      if (x.nums_.size() == 1 && x.nums_.back() == 0) {
        x.is_negative_ = false;
      }
      if (x.nums_.size() == 0) {
        nums_.push_back(0);
      }
    }

    BigInteger(std::string str) {
      if (str[0] == '-') {
        str = str.substr(1);
        is_negative_ = true;
      } else {
        is_negative_ = false;
      }
      for (long long i = str.length(); i > 0; i -= 9) {
        if (i < 9) {
          nums_.push_back(atoi(str.substr(0, i).c_str()));
        } else {
          nums_.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }
      }
      Balance(*this);
    }

    BigInteger &operator=(const BigInteger &a) {
      nums_ = a.nums_;
      is_negative_ = a.is_negative_;
      return *this;
    }

    BigInteger operator-() const {
      BigInteger a = *this;
      a.is_negative_ = !a.is_negative_;
      if (a.nums_.size() == 1 && a.nums_.back() == 0) {
        a.is_negative_ = false;
      }
      return a;
    }

    bool smaller(const BigInteger &x) const {
      if (is_negative_ != x.is_negative_) {
        return is_negative_;
      }
      if (nums_.size() != x.nums_.size()) {
        if (is_negative_) {
          return !(nums_.size() < x.nums_.size());
        } else {
          return nums_.size() < x.nums_.size();
        }
      }
      for (int i = nums_.size() - 1; i > -1; --i) {
        if (nums_[i] != x.nums_[i]) {
          if (is_negative_) {
            return !(nums_[i] < x.nums_[i]);
          } else {
            return nums_[i] < x.nums_[i];
          }
        }
      }
      return false;
    }

    explicit operator bool() const;

    BigInteger &operator+=(const BigInteger &x) {
      BigInteger a;
      if (is_negative_ && x.is_negative_) {
        a = -*this;
        a += -x;
        *this = -a;
        Balance(*this);
        return *this;
      }
      if (is_negative_) {
        a = x;
        a -= -*this;
        Balance(a);
        return *this = a;
      }
      if (x.is_negative_) {
        *this -= -x;
        Balance(*this);
        return *this;
      }
      int d = 0;
      for (int i = 0; i < int(std::max(nums_.size(), x.nums_.size())) || d > 0; ++i) {
        if (int(nums_.size()) == i) {
          nums_.push_back(0);
        }
        nums_[i] += d;
        if (int(x.nums_.size()) > i) {
          nums_[i] += x.nums_[i];
        }
        d = nums_[i] / base;
        nums_[i] %= base;
      }
      Balance(*this);
      return *this;
    }

    BigInteger &operator-=(const BigInteger &x) {
      BigInteger a;
      if (is_negative_ && x.is_negative_) {
        a = -*this;
        a -= -x;
        a = -a;
        *this = a;
        Balance(*this);
        return *this;
      }
      if (is_negative_) {
        a = -*this;
        a += x;
        *this = -a;
        Balance(*this);
        return *this;
      }
      if (x.is_negative_) {
        *this += -x;
        Balance(*this);
        return *this;
      }
      if (smaller(x)) {
        a = x;
        a -= *this;
        *this = -a;
        Balance(*this);
        return *this;
      }
      for (int i = 0; i < int(nums_.size()); ++i) {
        if (int(x.nums_.size()) > i) {
          nums_[i] -= x.nums_[i];
        }
        if (nums_[i] < 0) {
          nums_[i] += base;
          --nums_[i + 1];
        }
      }
      Balance(*this);
      return *this;
    }

    BigInteger &operator++() {
      *this += 1;
      return *this;
    }

    BigInteger &operator--() {
      *this -= 1;
      return *this;
    }

    BigInteger operator++(int) {
      BigInteger a = *this;
      ++*this;
      return a;
    }

    BigInteger operator--(int) {
      BigInteger a = *this;
      --*this;
      return a;
    }

    BigInteger &operator*=(const BigInteger &x) {
      BigInteger a;
      if (is_negative_ != x.is_negative_) {
        a.is_negative_ = true;
      }
      std::vector<long long> result(nums_.size() + x.nums_.size());
      for (int i = 0; i < int(nums_.size()); ++i) {
        int d = 0;
        for (int j = 0; j < int(x.nums_.size()) || d > 0; ++j) {
          result[i + j] += d;
          if (int(x.nums_.size()) > j) {
            result[i + j] += nums_[i] * x.nums_[j];
          }
          d = result[i + j] / base;
          result[i + j] %= base;
        }
      }
      a.nums_ = result;
      Balance(a);
      return *this = a;
    }

    void reverse(BigInteger &x) {
      for (int j = 0; j < int(x.nums_.size()) / 2; ++j) {
        std::swap(x.nums_[j], x.nums_[int(x.nums_.size()) - j - 1]);
      }
    }


    BigInteger &operator/=(const BigInteger &x) {
      BigInteger a;
      BigInteger b;
      BigInteger c = 0;
      if (c.nums_.size() != 0) {
        c.nums_.clear();
      }
      BigInteger result = 0;
      if (result.nums_.size() != 0) {
        result.nums_.clear();
      }
      if (is_negative_) {
        a = -*this;
      } else {
        a = *this;
      }
      if (x.is_negative_) {
        b = -x;
      } else {
        b = x;
      }
      if (is_negative_ != x.is_negative_) {
        result.is_negative_ = true;
      } else {
        result.is_negative_ = false;
      }
      for (int i = int(a.nums_.size()) - 1; i > -1; --i) {
        reverse(c);
        c.nums_.push_back(a.nums_[i]);
        reverse(c);
        Balance(c);
        BigInteger d;
        if (!(c.smaller(b))) {
          int left = 1;
          int right = base;
          while (right - left > 1) {
            int middle = (right + left) / 2;
            d = b;
            d *= middle;
            if (!(c.smaller(d))) {
              left = middle;
            } else {
              right = middle;
            }
          }
          result.nums_.push_back(left);
          d = b;
          d *= left;
          c -= d;
        } else {
          result.nums_.push_back(0);
        }
      }
      reverse(result);
      Balance(result);
      *this = result;
      return *this;
    }

    BigInteger &operator%=(const BigInteger &x) {
      BigInteger a = *this;
      a /= x;
      a *= x;
      *this -= a;
      Balance(*this);
      return *this;
    }

    std::string toString() const {
      std::string str;
      if (is_negative_) {
        str = "-";
      }
      if (nums_.size() != 0) {
        str += std::to_string(nums_[nums_.size() - 1]);
      }
      for (int i = nums_.size() - 2; i >= 0; --i) {
        int a = base / 10;
        int b = nums_[i];
        for (int j = 0; j < 9; ++j) {
          str += '0' + b / a;
          b %= a;
          a /= 10;
        }
      }
      return str;
    }

    double toDouble() const {
      double a = 0;
      double b = 1;
      for (size_t i = 0; i < size_t(nums_.size()); ++i) {
        a += nums_[i] * b;
        b *= base;
      }
      return a;
    }

private:
    std::vector<long long> nums_;
    bool is_negative_;
};

bool operator<(const BigInteger &a, const BigInteger &b) {
  return a.smaller(b);
}

bool operator>(const BigInteger &a, const BigInteger &b) {
  return b < a;
}

bool operator==(const BigInteger &a, const BigInteger &b) {
  return !(a < b || b < a);
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
  return !(a == b);
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
  return (a < b || a == b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
  return (a > b || a == b);
}

BigInteger::operator bool() const {
  return *this != 0;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
  BigInteger c = a;
  c += b;
  return c;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b) {
  BigInteger c = a;
  c -= b;
  return c;
}

BigInteger operator*(const BigInteger &a, const BigInteger &b) {
  BigInteger c = a;
  c *= b;
  return c;
}

BigInteger operator/(const BigInteger &a, const BigInteger &b) {
  BigInteger c = a;
  c /= b;
  return c;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b) {
  BigInteger c = a;
  c %= b;
  return c;
}

BigInteger operator ""_bi(unsigned long long a) {
  return BigInteger(a);
}

std::istream &operator>>(std::istream &in, BigInteger &a) {
  std::string str;
  in >> str;
  a = BigInteger(str);
  return in;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &a) {
  out << a.toString();
  return out;
}

BigInteger gcd(BigInteger a, BigInteger b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

class Rational {
public:

    Rational(int x) : numerator_(x), denominator_(1) {}

    Rational() : numerator_(0), denominator_(1) {}

    Rational(const BigInteger x, const BigInteger y = 1) : numerator_(x), denominator_(y) {}


    Rational &operator=(const Rational &x) {
      numerator_ = x.numerator_;
      denominator_ = x.denominator_;
      Balance();
      return *this;
    }

    Rational operator-() const {
      Rational x = *this;
      x.numerator_ = -x.numerator_;
      return x;
    }

    Rational &operator+=(const Rational &x) {
      numerator_ = numerator_ * x.denominator_ + x.numerator_ * denominator_;
      denominator_ = denominator_ * x.denominator_;
      Balance();
      return *this;
    }


    Rational &operator-=(const Rational &x) {
      *this += (-x);
      Balance();
      return *this;
    }

    Rational &operator*=(const Rational &x) {
      numerator_ *= x.numerator_;
      denominator_ *= x.denominator_;
      Balance();
      return *this;
    }

    Rational &operator/=(const Rational &x) {
      numerator_ *= x.denominator_;
      denominator_ *= x.numerator_;
      Balance();
      return *this;
    }

    std::string toString() {
      Balance();
      std::string str;
      str = numerator_.toString();
      if (denominator_ == 1) {
        return str;
      }
      str += '/' + denominator_.toString();;
      return str;
    }

    explicit operator double() const {
      double a = numerator_.toDouble();
      double b = denominator_.toDouble();
      return a / b;
    }

    std::string asDecimal(size_t precision = 0) {
      BigInteger n = numerator_;
      for (size_t i = 0; i < precision + 1; ++i) {
        n *= 10;
      }
      n /= denominator_;
      bool neg = false;
      if (n < 0) {
        neg = true;
        n = -n;
      }
      if (n % 10 >= 5) {
        n += 10;
      }
      n /= 10;
      std::string str = n.toString();

      for (int i = 0; i < int(str.size()) / 2; i++) {
        std::swap(str[i], str[str.size() - i - 1]);
      }

      while (precision > str.size()) {
        str += '0';
      }

      for (int i = 0; i < int(str.size()) / 2; i++) {
        std::swap(str[i], str[str.size() - i - 1]);
      }

      if (precision != 0 && precision <= str.size())
        str.insert(str.begin() + str.size() - precision, '.');

      if (str[0] == '.') {
        str = '0' + str;
      }
      if (neg) {
        str = '-' + str;
      }
      return str;
    }

    bool smaller(const Rational &x) const {
      return numerator_ * x.denominator_ < x.numerator_ * denominator_;
    }

private:
    BigInteger numerator_;
    BigInteger denominator_;

    void Balance() {
      BigInteger g = gcd(numerator_, denominator_);
      numerator_ /= g;
      denominator_ /= g;
      if (denominator_ < 0) {
        denominator_ = -denominator_;
        numerator_ = -numerator_;
      }
    }
};

Rational operator+(const Rational &a, const Rational &b) {
  Rational c(a);
  c += b;
  return c;
}

Rational operator-(const Rational &a, const Rational &b) {
  Rational c(a);
  c -= b;
  return c;
}

Rational operator*(const Rational &a, const Rational &b) {
  Rational c(a);
  c *= b;
  return c;
}

Rational operator/(const Rational &a, const Rational &b) {
  Rational c(a);
  c /= b;
  return c;
}

bool operator<(const Rational &a, const Rational &b) {
  return a.smaller(b);
}

bool operator>(const Rational &a, const Rational &b) {
  return b < a;
}

bool operator==(const Rational &a, const Rational &b) {
  return !(a < b || b < a);
}

bool operator!=(const Rational &a, const Rational &b) {
  return !(a == b);
}

bool operator<=(const Rational &a, const Rational &b) {
  return (a < b || a == b);
}

bool operator>=(const Rational &a, const Rational &b) {
  return (a > b || a == b);
}

