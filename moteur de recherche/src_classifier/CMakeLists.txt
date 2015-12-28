project(classifier)

add_library(classifier STATIC
			BayesianClassifier.h
			Domain.h
			BayesianClassifier.cpp
			Domain.cpp
)