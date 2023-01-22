SELECT NEW.user_id AS user_id,
tt.rowid AS transaction_type_id,
NEW.rowid AS action_id,
f.price AS amount
FROM meal_plans mp
INNER JOIN transaction_types tt
ON NEW.meal_plan_id = mp.rowid
