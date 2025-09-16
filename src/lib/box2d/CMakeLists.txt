set(BOX2D_SOURCE_FILES
	aabb.c
	aabb.h
	array.c
	array.h
	bitset.c
	bitset.h
	body.c
	body.h
	broad_phase.c
	broad_phase.h
	constraint_graph.c
	constraint_graph.h
	contact.c
	contact.h
	contact_solver.c
	contact_solver.h
	core.c
	core.h
	ctz.h
	distance.c
	distance_joint.c
	dynamic_tree.c
	geometry.c
	hull.c
	id_pool.c
	id_pool.h
	island.c
	island.h
	joint.c
	joint.h
	manifold.c
	math_functions.c
	motor_joint.c
	mouse_joint.c
	prismatic_joint.c
	revolute_joint.c
	shape.c
	shape.h
	solver.c
	solver.h
	solver_set.c
	solver_set.h
	stack_allocator.c
	stack_allocator.h
	table.c
	table.h
	timer.c
	types.c
	weld_joint.c
	wheel_joint.c
	world.c
	world.h
)

set(BOX2D_API_FILES
	../include/box2d/base.h
	../include/box2d/box2d.h
	../include/box2d/collision.h
	../include/box2d/id.h
	../include/box2d/math_functions.h
	../include/box2d/types.h
)

# Hide internal functions
# todo need to investigate this more
# https://gcc.gnu.org/wiki/Visibility
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(box2d ${BOX2D_SOURCE_FILES} ${BOX2D_API_FILES})

# Generate box2d_export.h to handles shared library builds
# turned this off to make Box2D easier to use without cmake
# include(GenerateExportHeader)
# generate_export_header(box2d)

target_include_directories(box2d
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_DEBUG_POSTFIX "d")

# Box2D uses C17
set_target_properties(box2d PROPERTIES
	C_STANDARD 17
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS YES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
	DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${BOX2D_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "include" FILES ${BOX2D_API_FILES})

add_library(box2d::box2d ALIAS box2d)

include(GNUInstallDirs)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS box2d
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)