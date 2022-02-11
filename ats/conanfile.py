from conans import ConanFile, tools, Meson
import os

class Rane(ConanFile):
    name = 'rane'
    version = '0.0.1'
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'pkg_config'
    exports_sources = 'src/*'
    requires = [ 'imgui/1.76'
               , 'vulkan-headers/1.2.140'
               , 'glm/0.9.9.8'
               ]

    def build(self):
        meson = Meson(self)
        meson.configure(source_folder=f'{self.source_folder}/src', build_folder='build')
        meson.build()

    def imports(self):
        self.copy('imgui.h', dst='src/third_party/cimgui/imgui', src='include')
        self.copy('imgui_internal.h', dst='src/third_party/cimgui/imgui', src='include')
        self.copy('imgui_impl_*', dst='bindings', src='res/bindings')
