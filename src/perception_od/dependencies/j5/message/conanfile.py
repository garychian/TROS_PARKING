from conans import ConanFile, tools
import os

class MessageConan(ConanFile):
    name = "message"
    version = "1.4.1"
    # No settings/options are necessary, this is header only
    no_copy_source = True

    def source(self):
        git = tools.Git(folder=self.source_folder)
        git.clone(
            "git@gitlab.hobot.cc:ptd/ap/middleware/message.git",
            "test-conan"
        )

    def package(self):
        self.copy(
            pattern="*",
            dst="include", src=os.path.join(self.source_folder, "include"),
            keep_path=True
        )

    def package_id(self):
        self.info.header_only()
