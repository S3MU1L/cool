add_executable(cool main.cpp)

target_include_directories(
	cool
	PRIVATE
	bytecode
	interpreter
	runtime
)