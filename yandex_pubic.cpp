#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <cfloat>
#include <tuple>
#include <numeric>

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */


// -------- Начало модульных тестов поисковой системы ----------


//	Добавление документов. 
//	Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
void TestAddDocument() {
	const int doc_id[] = { 0, 1 };
	const string docs[] = { "cat with a red bow"s, "red bow fight"s };
	const vector<int> doc_ratings[] = { { 3, 6 }, { 1, 7 } };

	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);
	assert(server.FindTopDocuments("cat"s).size() == 1);
	assert(server.FindTopDocuments("cat"s)[0].id == doc_id[0]);
	assert(server.FindTopDocuments("fight"s)[0].id != doc_id[0]);

	assert(server.FindTopDocuments("fight"s).size() == 1);
	assert(server.FindTopDocuments("fight"s)[0].id == doc_id[1]);
	assert(server.FindTopDocuments("cat"s)[0].id != doc_id[1]);
}

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
	const int doc_id = 42;
	const string content = "cat in the city"s;
	const vector<int> ratings = { 1, 2, 3 };
	// Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
	// находит нужный документ
	{
		SearchServer server;
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		const auto found_docs = server.FindTopDocuments("in"s);
		assert(found_docs.size() == 1);
		const Document& doc0 = found_docs[0];
		assert(doc0.id == doc_id);
	}

	// Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
	// возвращает пустой результат
	{
		SearchServer server;
		server.SetStopWords("in the"s);
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		assert(server.FindTopDocuments("in"s).empty());
	}
}

//	Поддержка минус-слов. 
//	Документы, содержащие минус-слова поискового запроса, не должны включаться в результаты поиска.
void TestMinusWords()
{
	const int doc_id[] = { 0, 1 };
	const string docs[] = { "cat with a red bow"s, "red bow fight"s };
	const vector<int> doc_ratings[] = { { 3, 6 }, { 1, 7 } };

	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);

	assert(server.FindTopDocuments("cat red"s).size() == 2);
	assert(server.FindTopDocuments("cat red -fight"s).size() == 1);
	assert(server.FindTopDocuments("cat -red"s).size() == 0);
}

//	Матчинг документов. При матчинге документа по поисковому запросу должны быть возвращены 
//	все слова из поискового запроса, присутствующие в документе. 
//	Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatch() {
	const int doc_id[] = { 0, 1 };
	const string docs[] = { "cat with a red bow"s, "red bow fight"s };
	const vector<int> doc_ratings[] = { { 3, 6 }, { 1, 7 } };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);
	auto [match, status] = server.MatchDocument("cat red bow"s, doc_id[0]);
	assert(match.size() == 3);
	auto [match2, status2] = server.MatchDocument("cat red -fight"s, doc_id[0]);
	assert(match2.size() == 2);
	auto [match3, status3] = server.MatchDocument("cat red -fight"s, doc_id[1]);
	assert(match3.size() == 0);
}

//	Сортировка найденных документов по релевантности. 
//	Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
void TestSortByRelevance() {
	const int doc_id[] = { 0, 1, 2 };
	const string docs[] = { "cat with a red bow"s, "bow fight"s, "red raccoon fight"s };
	const vector<int> doc_ratings[] = { { 3, 6 }, { 1, 7 }, { 10 } };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);
	server.AddDocument(doc_id[2], docs[2], DocumentStatus::ACTUAL, doc_ratings[2]);
	const auto founded_docs = server.FindTopDocuments("cat fight"s);

	// начиная со второго элемента: у элемента релевантность должна быть ниже чем у 
	// преспешника, но больше чем у последующего
	if (founded_docs.size() > 2)
	{
		for (size_t i = 1; i < founded_docs.size() - 1; i++)
		{
			assert(founded_docs[i - 1].relevance > founded_docs[i].relevance);
			assert(founded_docs[i].relevance > founded_docs[i + 1].relevance);
		}
	}
	else if (founded_docs.size() == 2)
	{
		// если они равны или первый больше второго
		assert(abs(founded_docs[0].relevance - founded_docs[0].relevance) < DBL_EPSILON ||
			founded_docs[0].relevance > founded_docs[1].relevance);
	}
}

//	Вычисление рейтинга документов. 
//	Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestRating() {
	const int doc_id[] = { 0, 1, 2 };
	const string docs[] = { "cat with a red bow"s, "bow fight"s, "red raccoon fight"s };
	const vector<int> doc_ratings[] = { { 3, 6, 4, 12, 0 }, { 1, -7, 14, 5 }, { 10, -3, 6, -12 } };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);
	server.AddDocument(doc_id[2], docs[2], DocumentStatus::ACTUAL, doc_ratings[2]);
	const auto founded_docs = server.FindTopDocuments("cat fight"s);
	for (size_t it = 0; it < founded_docs.size(); it++)
	{
		assert(founded_docs[it].rating == accumulate(doc_ratings[it].begin(), doc_ratings[it].end(), 0) / doc_ratings[it].size());
	}
}

//	Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
void TestPredicate() {
	const int doc_id[] = { 0, 1, 2, 3, 4 };
	const string docs[] = { "cat with a red bow"s, "bow fight"s, "red raccoon fight"s, "the raccoon night"s, "dog in the city"s };
	const vector<int> doc_ratings[] = { { 3, 6, 4 }, { 1 }, { 10, -3 }, { 1, 4}, { 5, -3 } };
	const vector<DocumentStatus> doc_status = { DocumentStatus::ACTUAL, DocumentStatus::ACTUAL,
	DocumentStatus::BANNED , DocumentStatus::IRRELEVANT, DocumentStatus::REMOVED };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], doc_status[0], doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], doc_status[1], doc_ratings[1]);
	server.AddDocument(doc_id[2], docs[2], doc_status[2], doc_ratings[2]);
	server.AddDocument(doc_id[3], docs[3], doc_status[3], doc_ratings[3]);
	server.AddDocument(doc_id[4], docs[4], doc_status[4], doc_ratings[4]);

	// подсчёт у кого из документов рейтинг больши или равен 3
	int rating_more_than_3 = 0,
		_id = 0,
		val = 0;
	for (size_t it = 0; it < doc_ratings->size(); it++)
	{
		for (const int i : doc_ratings[it])
		{
			val += i;
		}
		if (val >= 3)
			rating_more_than_3++;
		val = 0;
	}

	// подсчёт у кого из документов id елиться без остатка
	for (const int i : doc_id)
	{
		if (i % 2 == 0)
			_id++;
	}

	const string query_all_docs = "bow fight raccoon city"s;
	assert(server.FindTopDocuments(query_all_docs).size() == 2);	// ACTUAL
	assert(server.FindTopDocuments(query_all_docs,
		[](int document_id, DocumentStatus status, int rating)
		{return status == DocumentStatus::BANNED; }).size() == 1);
	assert(server.FindTopDocuments(query_all_docs,
		[](int document_id, DocumentStatus status, int rating)
		{return rating >= 3; }).size() == rating_more_than_3);
	assert(server.FindTopDocuments(query_all_docs,
		[](int document_id, DocumentStatus status, int rating)
		{return document_id % 2 == 0; }).size() == _id);
}

//	Поиск документов, имеющих заданный статус.
void TestFilteringWithStatus() {
	const int doc_id[] = { 0, 1, 2, 3, 4 };
	const string docs[] = { "cat with a red bow"s, "bow fight"s, "red raccoon fight"s, "the raccoon night"s, "dog in the city"s };
	const vector<int> doc_ratings[] = { { 3, 6, 4 }, { 1 }, { 10, -3 }, { 1, 4}, { 5, -3 } };
	const vector<DocumentStatus> doc_status = { DocumentStatus::ACTUAL, DocumentStatus::ACTUAL,
	DocumentStatus::BANNED , DocumentStatus::IRRELEVANT, DocumentStatus::REMOVED };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], doc_status[0], doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], doc_status[1], doc_ratings[1]);
	server.AddDocument(doc_id[2], docs[2], doc_status[2], doc_ratings[2]);
	server.AddDocument(doc_id[3], docs[3], doc_status[3], doc_ratings[3]);
	server.AddDocument(doc_id[4], docs[4], doc_status[4], doc_ratings[4]);
	const string query_all_docs = "bow fight raccoon city"s;

	//	сколько окументов с разными статусами
	map<DocumentStatus, size_t> status_count_on_docs;
	for (const DocumentStatus& status : doc_status)
	{
		status_count_on_docs[status]++;
	}

	assert(server.FindTopDocuments(query_all_docs, DocumentStatus::ACTUAL).size() == status_count_on_docs[DocumentStatus::ACTUAL]);
	assert(server.FindTopDocuments(query_all_docs, DocumentStatus::BANNED).size() == status_count_on_docs[DocumentStatus::BANNED]);
	assert(server.FindTopDocuments(query_all_docs, DocumentStatus::IRRELEVANT).size() == status_count_on_docs[DocumentStatus::IRRELEVANT]);
	assert(server.FindTopDocuments(query_all_docs, DocumentStatus::REMOVED).size() == status_count_on_docs[DocumentStatus::REMOVED]);
}

//	Корректное вычисление релевантности найденных документов.
void TestCorrectCalcRelevance()
{
	const int doc_id[] = { 0, 1, 2 };
	const string docs[] = { "cat with a red bow"s, "bow fight"s, "red raccoon fight"s };
	const vector<int> doc_ratings[] = { { 3, 6 }, { 1, 7 }, { 10 } };
	SearchServer server;
	server.AddDocument(doc_id[0], docs[0], DocumentStatus::ACTUAL, doc_ratings[0]);
	server.AddDocument(doc_id[1], docs[1], DocumentStatus::ACTUAL, doc_ratings[1]);
	server.AddDocument(doc_id[2], docs[2], DocumentStatus::ACTUAL, doc_ratings[2]);
	const auto founded_docs = server.FindTopDocuments("cat fight"s);
	double idf_cat = log(3. / 1.),
		idf_fight = log(3. / 2.);
	vector<double> relevance = { ((1. / 5) * idf_cat),
		((1. / 2.) * idf_fight),
		((1. / 3.) * idf_fight) };
	sort(relevance.begin(), relevance.end(), [](const double val1, const double val2) {return val1 > val2; });

	assert(abs(founded_docs[0].relevance - relevance[0]) < DBL_EPSILON);
	assert(abs(founded_docs[1].relevance - relevance[1]) < DBL_EPSILON);
	assert(abs(founded_docs[2].relevance - relevance[2]) < DBL_EPSILON);
}

// --------- Окончание модульных тестов поисковой системы -----------
void TestSearchServer() {
	TestExcludeStopWordsFromAddedDocumentContent();
	TestAddDocument();
	TestMinusWords();
	TestMatch();
	TestSortByRelevance();
	TestRating();
	TestPredicate();
	TestFilteringWithStatus();
	TestCorrectCalcRelevance();
}

int main() {
	TestSearchServer();
	// Если вы видите эту строку, значит все тесты прошли успешно
	std::cout << "Search server testing finished"s << std::endl;
}
