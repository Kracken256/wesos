/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-kernconf/Parser.hh>

using namespace wesos;
using namespace wesos::kernconf;

static auto get_view(const char* s) -> View<const u8> { return {bit_cast<const u8*>(s), strlen(s)}; }

TEST(Parser, EmptyFile) {
  const auto text = get_view("");

  EXPECT_TRUE(parse_kernel_config(text));
}

TEST(Parser, NewLine) {
  const auto text = get_view("\n");

  EXPECT_TRUE(parse_kernel_config(text));
}
