from conan import ConanFile, tools
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import files, get, copy, replace_in_file, collect_libs, rmdir, rm, apply_conandata_patches, export_conandata_patches
from conan.errors import ConanInvalidConfiguration
from conan.tools.apple import fix_apple_shared_install_name
from conan.tools.scm import Version
from conan.tools.build import check_min_cppstd
from conan.tools.microsoft import check_min_vs, is_msvc, is_msvc_static_runtime
import os

required_conan_version = ">=1.53.0"

class BcryptCpp(ConanFile):
    name = "bcryptcpp"
    version = "0.0.1"
    description = "A c++ wrapper for libxcrypt around bcrypt password hashing"
    license = "BSD-2-Clause"
    topics = ("crypto", "bcrypt")
    homepage = "https://github.com/xiadnoring/bcrypt-cpp"
    url = "https://github.com/conan-io/conan-center-index"
    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    exports_sources = "src/*", "include/*", "CMakeLists.txt", "LICENSE"

    @property
    def _min_cppstd(self):
        return 14

    @property
    def _compilers_minimum_version(self):
        return {
            "apple-clang": "10",
            "clang": "7",
            "gcc": "7",
            "msvc": "191",
            "Visual Studio": "15",
        }

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def validate(self):
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, self._min_cppstd)

        minimum_version = self._compilers_minimum_version.get(str(self.settings.compiler), False)
        if minimum_version and Version(self.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration(
                f"{self.ref} requires C++{self._min_cppstd}, which your compiler does not support."
            )

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["WITH_CONAN"] = True
        tc.generate()

    def requirements(self):
        self.requires("libxcrypt/4.4.36")
        self.requires("fmt/11.0.2")

    def export_sources(self):
        pass
        #export_conandata_patches(self)

    def _patch_sources(self):
        apply_conandata_patches(self)

    def build(self):
        # self._patch_sources()

        cmake = CMake(self)

        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(self, "LICENSE", src=os.path.join(self.source_folder), dst=os.path.join(self.package_folder, "licenses"))

        fix_apple_shared_install_name(self)

    def package_info(self):

        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "bcryptcpp")
        self.cpp_info.set_property("cmake_target_name", "bcryptcpp::bcryptcpp")
        self.cpp_info.set_property("pkg_config_name", "bcryptcpp")

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["dl", "m", "pthread"]
        elif self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "shlwapi"]

        self.cpp_info.libs = collect_libs(self)
