-- Вывести актеров, чей рейтинг выше 4.5 и возраст равен 21.
SELECT full_name
FROM actors
WHERE rating > 4.5
	AND age = 21;