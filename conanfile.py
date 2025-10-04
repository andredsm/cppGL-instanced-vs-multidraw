from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain
from conan.tools.files import copy
import os


class OpenGLRendererConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("glfw/3.3.8")
        self.requires("glew/2.2.0")
        self.requires("glm/0.9.9.8")
        self.requires("opengl/system")
        self.requires("imgui/1.89.9")

    def configure(self):
        self.options["glfw"].shared = False
        self.options["glew"].shared = False
        self.options["imgui"].shared = False

    def generate(self):
        info = self.dependencies["imgui"].cpp_info.srcdirs
        # copy to the build folder the imgui backends
        copy(
            self,
            "*glfw*",
            src=info[0],
            dst=os.path.join(self.build_folder, "imgui_backends"),
        )

        copy(
            self,
            "*opengl3*",
            src=info[0],
            dst=os.path.join(self.build_folder, "imgui_backends"),
        )
