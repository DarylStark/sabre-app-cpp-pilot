from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain


class SabreAppPilotConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    requires = (
        "glfw/3.4",
        "imgui/1.92.8-docking",
        "gtest/1.17.0",
        "tomlplusplus/3.4.0"
    )

    default_options = {
        "imgui/*:with_glfw": True,
        "imgui/*:with_opengl3": True,
    }

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()