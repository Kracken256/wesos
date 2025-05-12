/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  class SourceLocation {
  public:
    using LineNumber = decltype(__builtin_LINE());
    using ColumnNumber = decltype(__builtin_COLUMN());
    using FileName = decltype(__builtin_FILE());
    using FunctionName = decltype(__builtin_FUNCTION());

    constexpr SourceLocation(FileName file = __builtin_FILE(), FunctionName function = __builtin_FUNCTION(),
                             LineNumber line = __builtin_LINE(), ColumnNumber column = __builtin_COLUMN())
        : m_file(file), m_function(function), m_line(line), m_column(column) {}

    static constexpr auto current(FileName file = __builtin_FILE(), FunctionName function = __builtin_FUNCTION(),
                                  LineNumber line = __builtin_LINE(),
                                  ColumnNumber column = __builtin_COLUMN()) -> SourceLocation {
      return {file, function, line, column};
    }

    [[nodiscard]] constexpr auto file_name() const -> FileName { return m_file; }
    [[nodiscard]] constexpr auto function_name() const -> FunctionName { return m_function; }
    [[nodiscard]] constexpr auto line_number() const -> LineNumber { return m_line; }
    [[nodiscard]] constexpr auto column_number() const -> ColumnNumber { return m_column; }

  private:
    FileName m_file;
    FunctionName m_function;
    LineNumber m_line;
    ColumnNumber m_column;
  };
}  // namespace wesos
