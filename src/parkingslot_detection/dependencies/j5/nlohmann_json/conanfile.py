import os

from conans import ConanFile, tools


class NlohmannJsonConan(ConanFile):
    name = "nlohmann_json"
    version = "3.7.3"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of NlohmannJson here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def source(self):
        '''retrieval of the source code here. Remember you can also put the code
        in the folder and use exports instead of retrieving it with this
        source() method
        '''
        # self.run("git clone ...") or
        # tools.download("url", "file.zip")
        # tools.unzip("file.zip" )

    def package(self):
        self.copy("*.h", "include")

    def package_id(self):
        self.info.header_only()
