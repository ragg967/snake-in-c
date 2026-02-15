add_rules("mode.debug", "mode.release")
add_requires("raylib 5.5")

target("snake")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("raylib")
