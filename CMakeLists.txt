cmake_minimum_required (VERSION 2.8.12)
project (TEXT_CLASSIFIER)

include_directories(include)
add_library(text-classifier
  src/FrequencyHashTable.cpp
  src/FrequencyList.cpp
  src/Heap.cpp
  src/KNNClassifier.cpp
  src/LogisticRegressionClassifier.cpp
  src/LogisticRegressionTrainer.cpp
  src/NaiveBayesClassifier.cpp
  src/NaiveBayesTrainer.cpp
  src/PerceptronClassifier.cpp
  src/PerceptronTrainer.cpp
  src/ProbabilisticNeuralNetwork.cpp
  src/ProbabilityHashTable.cpp
  src/ProbabilityList.cpp
  src/WordHashTable.cpp
  src/WordList.cpp
  src/DocumentTransformer.cpp
)

add_executable (classifier-test test.cpp)
target_link_libraries(classifier-test text-classifier)
