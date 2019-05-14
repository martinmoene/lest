from conans import ConanFile, CMake

class LestConan(ConanFile):
    version = "1.35.1"
    name = "lest"
    description = "A modern, C++11-native, single-file header-only, tiny framework for unit-tests, TDD and BDD (includes C++98 variant)"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    url = "https://github.com/martinmoene/lest.git"
    exports_sources = "include/lest/*", "LICENSE.txt"
    settings = "compiler", "build_type", "arch"
    build_policy = "missing"
    author = "Martin Moene"

    def build(self):
        """Avoid warning on build step"""
        pass

    def package(self):
        """Provide pkg/include/nonstd/*.hpp"""
        self.copy("*.hpp")

    def package_info(self):
        self.info.header_only()
