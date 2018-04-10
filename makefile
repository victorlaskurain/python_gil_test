THREAD_COUNT ?= 1 2 4
run: cpp_test python_test_mp.py
	@$(foreach tc,$(THREAD_COUNT),./cpp_test          $(tc);)
	@$(foreach tc,$(THREAD_COUNT),./python_test_mt.py $(tc);)
	@$(foreach tc,$(THREAD_COUNT),./python_test_mp.py $(tc);)
cpp_test: cpp_test.cpp
	@$(CXX) -Wall -O3 -std=c++11 $^ -lpthread -o $@
