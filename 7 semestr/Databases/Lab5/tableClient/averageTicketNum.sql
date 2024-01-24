-- Вывести имена клиентов и средний номер билета для каждого клиента.
SELECT full_name,
	AVG(ticket_number) AS average_ticket_number
FROM client
GROUP BY full_name;