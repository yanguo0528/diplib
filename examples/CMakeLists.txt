set(CMAKE_BUILD_WITH_INSTALL_RPATH 0) # Allow running these in the build directory -- they're not installed anyway

# An example program that demonstrates different ways of doing per-pixel arithmetic
add_executable(image_arithmetic cpp/image_arithmetic.cpp)
target_link_libraries(image_arithmetic DIP)

# An example program that demonstrates how to register a new measurement feature in dip::MeasurementTool
add_executable(register_measurement_feature cpp/register_measurement_feature.cpp)
target_link_libraries(register_measurement_feature DIP)

# A program that times dilations with different structuring element shapes and sizes
add_executable(time_dilations cpp/time_dilations.cpp)
target_link_libraries(time_dilations DIP)

# A program that times the FFT for different image sizes
add_executable(time_fft cpp/time_fft.cpp)
target_link_libraries(time_fft DIP)

# A program that shows the difference between dip::VarianceAccumulator and dip::FastVarianceAccumulator
add_executable(variance cpp/variance.cpp)
target_link_libraries(variance DIP)

# A program to compute the fractal dimension of an input image
add_executable(fractal_dimension cpp/fractal_dimension.cpp)
target_link_libraries(fractal_dimension DIP)

# This is a simple program that replicates the dip::RadialMean function
add_executable(radial_mean cpp/radial_mean.cpp)
target_compile_definitions(radial_mean PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(radial_mean DIP)

# This is a program that demonstates how to generate various shapes
add_executable(generate_shapes cpp/generate_shapes.cpp)
target_link_libraries(generate_shapes DIP)

# A program to time functions with and without multithreading, useful to fine-tune the cost value that determines
# for what size image it is worth using multithreading.
add_executable(time_openmp cpp/time_openmp.cpp)
target_link_libraries(time_openmp DIP)

# A program that tests the binary morphology functions
add_executable(test_binary cpp/test_binary.cpp)
target_compile_definitions(test_binary PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(test_binary DIP)

# The `examples` target builds them all
add_custom_target(examples DEPENDS
                  image_arithmetic
                  register_measurement_feature
                  time_dilations
                  variance
                  fractal_dimension
                  radial_mean
                  time_openmp
                  test_binary
                  )

## A set of examples that need DIPviewer, compile only if DIPviewer is available

if(DIP_BUILD_DIPVIEWER)

   # A test program that demonstrates some of the low-level functionality of DIPviewer
   add_executable(viewer cpp/viewer.cpp)
   target_compile_definitions(viewer PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(viewer DIP DIPviewer Threads::Threads)

   # An example program that does simple image manipulation and uses dip::viewer::ShowSimple
   add_executable(display cpp/display.cpp)
   target_compile_definitions(display PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(display DIP DIPviewer)

   # A simple tool that shows an image file using dip::viewer::Show
   add_executable(fileviewer cpp/fileviewer.cpp)
   target_link_libraries(fileviewer DIP DIPviewer)
   if(DIP_BUILD_JAVAIO)
      target_link_libraries(fileviewer DIPjavaio)
   endif()

   # An example program that shows how to delete small objects from a grey-value image, and use dip::viewer::ShowSimple
   add_executable(select_objects cpp/select_objects.cpp)
   target_compile_definitions(select_objects PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(select_objects DIP DIPviewer)

   # An example program that shows how to apply a convolution with a custom kernel to an image
   add_executable(filter_image cpp/filter_image.cpp)
   target_compile_definitions(filter_image PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(filter_image DIP DIPviewer)

   # An example program that shows how to use various geometric transformations
   add_executable(geometric_transformation cpp/geometric_transformation.cpp)
   target_compile_definitions(geometric_transformation PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(geometric_transformation DIP DIPviewer)

   # An example program that uses superpixel segmentation and graph manipulation
   add_executable(oversegmentation cpp/oversegmentation.cpp)
   target_compile_definitions(oversegmentation PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
   target_link_libraries(oversegmentation DIP DIPviewer)

   add_dependencies(examples
                    viewer
                    display
                    fileviewer
                    select_objects
                    filter_image
                    geometric_transformation
                    )

   ## An example that needs OpenCV, compile only if OpenCV is available

   find_package(OpenCV QUIET COMPONENTS core imgproc highgui)
   if(OpenCV_FOUND)

      # An example program that uses both DIPlib and OpenCV
      add_executable(opencv_with_dip external_interfaces/opencv_with_dip.cpp)
      target_compile_definitions(opencv_with_dip PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
      target_link_libraries(opencv_with_dip DIP DIPviewer ${OpenCV_LIBS})

   endif()

   ## An example that needs Vigra, compile only if Vigra is available

   find_package(Vigra QUIET)
   if(Vigra_FOUND)

      # An example program that uses both DIPlib and Vigra
      add_executable(vigra_with_dip external_interfaces/vigra_with_dip.cpp)
      target_compile_definitions(vigra_with_dip PRIVATE DIP__EXAMPLES_DIR="${CMAKE_CURRENT_LIST_DIR}")
      target_include_directories(vigra_with_dip PRIVATE ${Vigra_INCLUDE_DIRS})
      target_link_libraries(vigra_with_dip DIP DIPviewer vigraimpex)
      if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
         target_compile_options(vigra_with_dip PRIVATE -Wno-conversion -Wno-sign-conversion)
      elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
         target_compile_options(vigra_with_dip PRIVATE -Wno-conversion -Wno-sign-conversion -Wno-deprecated-copy -Wno-parentheses)
      endif()

   endif()

endif(DIP_BUILD_DIPVIEWER)
