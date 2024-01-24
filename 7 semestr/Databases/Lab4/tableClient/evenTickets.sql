-- Вывести имена клиентов, у которых номер билета четный.
SELECT full_name
FROM client
WHERE ticket_number % 2 = 0;