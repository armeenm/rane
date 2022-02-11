project('RANE', 'c', 'cpp', version: '0.0.1')

### CImGui ###
add_project_arguments(
    '-DIMGUI_IMPL_API= '
  , '-DATS_MEMALLOC_LIBC'
  , '-Wno-unused-label'
  , '-Wno-unused-but-set-variable'
  , language: 'c'
  )
add_project_arguments('-DIMGUI_IMPL_API=extern "C"', language: 'cpp')

bindings_dir = meson.current_build_dir() + '/../bindings'
bindings_src = [
    bindings_dir + '/imgui_impl_glfw.cpp'
  , bindings_dir + '/imgui_impl_opengl3.cpp'
  ]

cimgui_dir = 'third_party/cimgui'
cimgui_build_dir = meson.current_build_dir() + '/../src/third_party/cimgui'
cimgui_inc = include_directories(bindings_dir, cimgui_dir, cimgui_build_dir)

cimgui_lib = library('cimgui'
  , [cimgui_dir + '/cimgui.cpp', bindings_src]
  , include_directories: cimgui_inc
  , dependencies: [dependency('glfw3'), dependency('glew'), dependency('imgui')]
  )

cimgui = declare_dependency(
    link_with: cimgui_lib
  , include_directories: cimgui_inc
  )

### ATS ###
patscc = find_program('patscc')

cc = meson.get_compiler('c')

atslib = cc.find_library('atslib', dirs: '/usr/local/lib/ats2-postiats-0.4.0/ccomp/atslib/lib')

ats_src = custom_target('ATS to C Compilation'
  , output: ['main_dats.c', 'glfw_dats.c', 'glew_dats.c', 'cimgui_dats.c', 'gui_dats.c']
  , input: ['main.dats', 'glfw.dats', 'glew.dats', 'cimgui.dats', 'gui.dats']
  , command: [patscc, '-IATS', meson.current_source_dir(),
              '-ccats', '@INPUT@']
  , install: false
  )

ats_inc = include_directories(
    '/usr/local/lib/ats2-postiats-0.4.0'
  , '/usr/local/lib/ats2-postiats-0.4.0/ccomp/runtime'
  , is_system: true
  )

### RANE ###
executable('rane'
  , [ats_src]
  , include_directories: [ats_inc]
  , dependencies: [atslib, cimgui, dependency('glfw3'), dependency('glew')]
  , override_options: ['c_std=gnu99']
  , link_language: 'c'
  )
