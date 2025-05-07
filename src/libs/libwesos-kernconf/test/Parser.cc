/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-kernconf/Parser.hh>

using namespace wesos::kernconf;

TEST(Parser, EmptyFile) {  //
  EXPECT_FALSE(parse_kernel_config({}));
}
