add_definitions(-DFILE_PATH="${CMAKE_CURRENT_LIST_DIR}/")
add_definitions(-DCHAOS_LIB)
add_executable(lab1_kMean k_mean.cpp)


find_package(OpenCV REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
set(GL_LIB ${OPENGL_LIBRARIES})
set(GLU_LIB)
set(GLUT_LIB ${GLUT_LIBRARY})

# add chaos lib
file(GLOB CHAOS_LIB "/home/bloodyangel/git_hub/ChaosAndFractal_Lib/lib/Release/*.a")
target_link_libraries(lab1_kMean ${CHAOS_LIB} ${OpenCV_LIBS} ${GL_LIB} ${GLU_LIB} ${GLUT_LIB})
target_include_directories(lab1_kMean PUBLIC "/home/bloodyangel/git_hub/ChaosAndFractal_Lib/include")
