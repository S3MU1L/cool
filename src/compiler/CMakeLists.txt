add_executable(coolc main.cpp)

target_include_directories(
	coolc
	PRIVATE
	codegen
	ir
	lexer
	parser
)