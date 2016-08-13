#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "NaiveBayesTrainer.hpp"
#include "NaiveBayesClassifier.hpp"
#include "PerceptronTrainer.hpp"
#include "PerceptronClassifier.hpp"
#include "KNNClassifier.hpp"
#include "LogisticRegressionTrainer.hpp"
#include "LogisticRegressionClassifier.hpp"
#include "ProbabilisticNeuralNetwork.hpp"
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

vector<string> categories;
vector<string> documents;
vector<string> testSet;
vector<string> testCategories;

string trainingFile = "";
string testFile = "";
bool useBayes = false;
bool useKnn = false;
bool usePerceptron = false;
bool useLogistic = false;
bool usePnn = false;

void readTrainingSetFromFile(const char* fileName);
void readTestSetFromFile(const char* fileName);
void testNaiveBayes();
void testKnn();
void testPerceptron();
void testLogisticRegression();
void testPnn();
void printUsageMessage(string executable);
int processCommandLineArguments(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int sufficientArguments = processCommandLineArguments(argc, argv);
    if (sufficientArguments == -1)
    {
        printUsageMessage(argv[0]);
        return 0;
    }

    readTrainingSetFromFile(trainingFile.c_str());
    readTestSetFromFile(trainingFile.c_str());

    if (useBayes)
        testNaiveBayes();

    if (useKnn)
        testKnn();

    if (usePerceptron)
        testPerceptron();

    if (useLogistic)
        testLogisticRegression();

    if (usePnn)
        testPnn();

	return 0;
}

void readTrainingSetFromFile(const char* fileName)
{
	ifstream reader;
	string line;

	reader.open(fileName);
	while(!reader.eof())
	{
		getline(reader,line);

		unsigned int i=0;
		while(line[i] != '\t')
			i++;

		categories.push_back(line.substr(0,i));
		documents.push_back(line.substr(i+1,line.size()-i-2));
	}
	reader.close();
}

void readTestSetFromFile(const char* fileName)
{
	ifstream reader;
	string line;

	reader.open(fileName);
	while(!reader.eof())
	{
		getline(reader,line);

		unsigned int i=0;
		while(line[i] != '\t')
			i++;

		testCategories.push_back(line.substr(0,i));
		testSet.push_back(line.substr(i+1,line.size()-i-2));
	}
	reader.close();
}

void testNaiveBayes()
{
	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

    cout << "Naive Bayes test in progress...\n";
	NaiveBayesTrainer* naiveBayes = new NaiveBayesTrainer(categories,documents);
	naiveBayes->trainClassifier();

	NaiveBayesClassifier* naiveBayesClassifier = new NaiveBayesClassifier();
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = naiveBayesClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Naive Bayes - ratio of correctly classified documents: " << correctlyClassifiedPercent << endl;
}

void testKnn()
{
	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

    cout << "KNN test in progress...\n";
	KNNClassifier* knnClassifier = new KNNClassifier(documents, categories, 20);
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = knnClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "KNN - ratio of correctly classified documents: " << correctlyClassifiedPercent << endl;
}

void testPerceptron()
{
	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

    cout << "Perceptron test in progress...\n";
	PerceptronTrainer* perceptronTrainer = new PerceptronTrainer(documents,categories);
	perceptronTrainer->trainClassifier();

	PerceptronClassifier* perceptronClassifier = new PerceptronClassifier();
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = perceptronClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}

	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Perceptron - ratio of correctly classified documents: " << correctlyClassifiedPercent << endl;
}

void testLogisticRegression()
{
	DocumentTransformer* documentTransformer = new DocumentTransformer(documents, categories, "ascii");
    cout << "Transforming documents...\n";
	documentTransformer->transformDocuments();
	cout << "Transformation complete\n";

	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

    cout << "Logistic regression test in progress...\n";
	LogisticRegressionTrainer* logisticRegressionTrainer = new LogisticRegressionTrainer(documents, categories, "ascii");
	logisticRegressionTrainer->trainClassifier();

	LogisticRegressionClassifier* logisticRegressionClassifier = new LogisticRegressionClassifier("ascii");
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = logisticRegressionClassifier->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}
	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Logistic regression - ratio of correctly classified documents: " << correctlyClassifiedPercent << endl;
}

void testPnn()
{
    cout << "Transforming documents...\n";
	DocumentTransformer* documentTransformer = new DocumentTransformer(documents, categories, "ascii");
	documentTransformer->transformDocuments();
	cout << "Transformation complete\n";

	double correctlyClassified = 0.0;
    double correctlyClassifiedPercent = 0.0;
	string category = "";

    cout << "Probabilistic neural network test in progress...\n";
    ProbabilisticNeuralNetwork* probabilisticNeuralNetwork = new ProbabilisticNeuralNetwork(documents.size(), categories, "ascii", 25.0f);
	for(unsigned int i=0; i<testSet.size(); i++)
	{
		category = probabilisticNeuralNetwork->classifyDocument(testSet[i]);
		if(category == testCategories[i])
			correctlyClassified += 1;
	}
	correctlyClassifiedPercent = correctlyClassified / testSet.size();
	cout << "Probabilistic neural network - ratio of correctly classified documents: " << correctlyClassifiedPercent << endl;
}

int processCommandLineArguments(int argc, char* argv[])
{
    int fileCounter = 0;
    int optionCounter = 0;
    for (int i=1; i<argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (fileCounter == 0)
            {
                trainingFile = argv[i];
            }
            else if (fileCounter == 1)
            {
                testFile = argv[i];
            }
            else
            {
                return -1;
            }
            fileCounter += 1;
        }
        else
        {
            string option = argv[i];
            option = option.substr(1, option.size());
            if (option == "bayes")
            {
                useBayes = true;
            }
            else if (option == "knn")
            {
                useKnn = true;
            }
            else if (option == "perceptron")
            {
                usePerceptron = true;
            }
            else if (option == "logistic")
            {
                useLogistic = true;
            }
            else if (option == "pnn")
            {
                usePnn = true;
            }
            else
            {
                cout << "Unknown option " << option << endl;
                return -1;
            }

            optionCounter += 1;
        }
    }

    if (fileCounter != 2)
        return -1;

    if (optionCounter == 0)
    {
        useBayes = true;
        useKnn = true;
        usePerceptron = true;
        useLogistic = true;
        usePnn = true;
    }

    return 0;
}

void printUsageMessage(string executable)
{
    cout << "Usage: " << executable << " <trainingFileName> <testFileName>\n"
         << "OPTIONS: -bayes      -- Trains and tests a naive Bayes classifier\n"
         << "         -knn        -- Tests a k nearest neighbours classifier with 20 neighbours\n"
         << "         -perceptron -- Trains and tests a perceptron\n"
         << "         -logistic   -- Trains and tests a logistic regression classifier (typically a slow process)\n"
         << "         -pnn        -- Tests a probabilistic neural network classifier (typically a slow process)\n"
         << "If no options are specified, all five classifiers will be used.\n";
}
