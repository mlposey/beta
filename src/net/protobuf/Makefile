# This makefile generates service and message C++ code for a beta.proto definition file.
all:
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` beta.proto
	protoc --cpp_out=. beta.proto