import os
from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain


class SabreAppPilotConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    

    default_options = {
        "imgui/*:with_glfw": True,
        "imgui/*:with_opengl3": True,
    }
    
    def requirements(self):
        skips = os.getenv("CONAN_SKIPS", default='').split()
        requires = (
            "glfw/3.4",
            "imgui/1.92.8-docking",
            "gtest/1.17.0",
            "tomlplusplus/3.4.0",
            "cli11/2.6.2",
            "asio/1.38.0"
        )
        
        for requirement in requires:
            if requirement.split('/')[0] in skips:
                continue
            self.requires(requirement)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()