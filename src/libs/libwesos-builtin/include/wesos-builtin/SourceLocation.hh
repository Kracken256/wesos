/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  /**
   * @class SourceLocation
   * @brief A utility class to capture and represent source code location information.
   *
   * This class provides a way to capture the file name, function name, line number,
   * and column number of a specific location in the source code. It leverages compiler
   * built-in functions to retrieve this information at compile time.
   */
  class SourceLocation {
  public:
    /** @brief Represents the type of a line number in the source code. */
    using LineNumber = decltype(__builtin_LINE());

    /** @brief Represents the type of a column number in the source code. */
    using ColumnNumber = decltype(__builtin_COLUMN());

    /** @brief Represents the type of a file name in the source code. */
    using FileName = decltype(__builtin_FILE());

    /** @brief Represents the type of a function name in the source code. */
    using FunctionName = decltype(__builtin_FUNCTION());

    /**
     * @brief Default constructor for SourceLocation.
     *
     * Initializes the source location with empty strings and zero values.
     */
    constexpr SourceLocation() : m_file(""), m_function(""), m_line(0), m_column(0) {}

    /**
     * @brief Constructs a SourceLocation object with the specified source code location details.
     *
     * @param file The name of the file.
     * @param function The name of the function.
     * @param line The line number in the file.
     * @param column The column number in the file.
     */
    constexpr SourceLocation(FileName file, FunctionName function, LineNumber line, ColumnNumber column)
        : m_file(file), m_function(function), m_line(line), m_column(column) {}

    constexpr SourceLocation(const SourceLocation&) = default;
    constexpr SourceLocation(SourceLocation&&) = default;
    constexpr auto operator=(const SourceLocation&) -> SourceLocation& = default;
    constexpr auto operator=(SourceLocation&&) -> SourceLocation& = default;
    constexpr ~SourceLocation() = default;

    /**
     * @brief Creates a SourceLocation object representing the current source code location.
     *
     * @param file The name of the file. Defaults to the current file name.
     * @param function The name of the function. Defaults to the current function name.
     * @param line The line number in the file. Defaults to the current line number.
     * @param column The column number in the file. Defaults to the current column number.
     * @return A SourceLocation object representing the current source code location.
     */
    static constexpr auto current(FileName file = __builtin_FILE(), FunctionName function = __builtin_FUNCTION(),
                                  LineNumber line = __builtin_LINE(),
                                  ColumnNumber column = __builtin_COLUMN()) -> SourceLocation {
      return {file, function, line, column};
    }

    /**
     * @brief Retrieves the file name of the source code location.
     * @return The file name as a FileName type. It is never nullptr.
     */
    [[nodiscard]] constexpr auto file_name() const -> FileName { return m_file; }

    /**
     * @brief Retrieves the function name of the source code location.
     * @return The function name as a FunctionName type. It is never nullptr.
     */
    [[nodiscard]] constexpr auto function_name() const -> FunctionName { return m_function; }

    /**
     * @brief Retrieves the line number of the source code location.
     * @return The line number as a LineNumber type.
     */

    [[nodiscard]] constexpr auto line_number() const -> LineNumber { return m_line; }

    /**
     * @brief Retrieves the column number of the source code location.
     * @return The column number as a ColumnNumber type.
     */
    [[nodiscard]] constexpr auto column_number() const -> ColumnNumber { return m_column; }

  private:
    FileName m_file;
    FunctionName m_function;
    LineNumber m_line;
    ColumnNumber m_column;
  };
}  // namespace wesos
