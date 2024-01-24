-- Вывести имена клиентов и соответствующие им номера билетов,
-- упорядоченные по номерам билетов.
SELECT full_name,
	ticket_number
FROM client
ORDER BY ticket_number;