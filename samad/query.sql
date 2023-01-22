SELECT t.datetime AS date_and_time,
tt.name AS transaction_type_name,
t.amount AS amount
FROM transactions t
INNER JOIN transaction_types tt
ON t.transaction_type_id = tt.rowid
WHERE t.user_id = 4;
