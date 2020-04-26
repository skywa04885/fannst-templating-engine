CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(fannst-templating-engine VERSION 0.0.1)

ADD_LIBRARY(
		lexer

		src/lexer.hpp
		src/lexer.cpp
)

ADD_LIBRARY(
		prepare

		src/prepare.src.cpp
		src/prepare.src.hpp
)

ADD_LIBRARY(
		errors

		src/errors.src.cpp
		src/errors.src.hpp
)

ADD_EXECUTABLE(
		${PROJECT_NAME}

		test.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} lexer prepare errors)
