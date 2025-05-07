include(clang-global/common)

################################################################################
# OPTIMIZATION
################################################################################
add_compile_options(-O3 -DNDEBUG)

if(WESOS_WORLD_LTO)
  add_compile_options(-flto)
  add_link_options(-flto)
endif()
