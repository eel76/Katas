#pragma once
#include <functional>
#include <string>
#include <utility>

namespace game
{
  auto count(std::string text, std::string pattern, size_t pos = 0)
  {
    auto const matchingPos = text.find(pattern, pos);
    if (matchingPos == std::string::npos)
      return 0;

    return 1 + count(text, pattern, matchingPos + 1);
  }

  auto contains(std::string text, std::string pattern)
  {
    return count(text, pattern) >= 1;
  }

  auto is_multiple_of(int number)
  {
    return [=](auto value) {
      return (value % number) == 0;
    };
  }

  auto operator"" _if(char const* text, size_t)
  {
    return [=](bool condition) {
      if (condition)
        return std::string{ text };

      return std::string{};
    };
  }

  template <class... Rules>
  auto compile(Rules&&... rules)
  {
    return[=](int number) {
      auto value = std::string{};
      return ((value = std::invoke(rules, value, number)), ...);
    };
  }

  inline namespace version1
  {
    auto fizzRule()
    {
      return [](auto result, auto number) {
        return result + "Fizz"_if(is_multiple_of(3)(number));
      };
    }

    auto buzzRule()
    {
      return [](auto result, auto number) {
        return result + "Buzz"_if(is_multiple_of(5)(number));
      };
    }

    auto toStringRule()
    {
      return [](auto result, auto number) {
        if (result.empty())
          return std::to_string(number);

        return result;
      };
    }

    auto fizzBuzz(int number)
    {
      return std::invoke(compile(fizzRule(), buzzRule (), toStringRule ()), number);
    }
  }

  namespace version2
  {
    auto fizzRule()
    {
      return [](auto result, int number) {
        return result + "Fizz"_if(is_multiple_of(3)(number) || contains(std::to_string(number), std::to_string(3)));
      };
    }

    auto buzzRule()
    {
      return [](auto result, int number) {
        return result + "Buzz"_if(is_multiple_of(5)(number) || contains(std::to_string(number), std::to_string(5)));
      };
    }

    using version1::toStringRule;

    auto fizzBuzz(int number)
    {
      return std::invoke(compile(fizzRule(), buzzRule(), toStringRule()), number);
    }
  }
}