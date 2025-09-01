CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
TARGET = shamirs_secret
SOURCE = main.cpp
INPUT = input.json

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

run: $(TARGET) $(INPUT)
	./$(TARGET)

test1: $(TARGET)
	@echo "Testing with sample case 1..."
	@echo '{"keys":{"n":4,"k":3},"1":{"base":"10","value":"4"},"2":{"base":"2","value":"111"},"3":{"base":"10","value":"12"},"6":{"base":"4","value":"213"}}' > test1.json
	@cp test1.json input.json
	@./$(TARGET)
	@rm -f test1.json

test2: $(TARGET)
	@echo "Testing with sample case 2..."
	@echo '{"keys":{"n":10,"k":7},"1":{"base":"6","value":"13444211440455345511"},"2":{"base":"15","value":"aed7015a346d635"},"3":{"base":"15","value":"6aeeb69631c227c"},"4":{"base":"16","value":"e1b5e05623d881f"},"5":{"base":"8","value":"316034514573652620673"},"6":{"base":"3","value":"2122212201122002221120200210011020220200"},"7":{"base":"3","value":"20120221122211000100210021102001201112121"},"8":{"base":"6","value":"20220554335330240002224253"},"9":{"base":"12","value":"45153788322a1255483"},"10":{"base":"7","value":"1101613130313526312514143"}}' > test2.json
	@cp test2.json input.json
	@./$(TARGET)
	@rm -f test2.json

debug: $(SOURCE)
	$(CXX) $(CXXFLAGS) -DDEBUG -g -o $(TARGET) $(SOURCE)

.PHONY: all clean run test1 test2 debug