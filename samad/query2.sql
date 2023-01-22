CREATE TRIGGER AfterReserve
AFTER INSERT ON reservations
BEGIN 
INSERT INTO transactions (
user_id,
transaction_type_id,
action_id,
amount,
datetime) 
SELECT r.user_id AS user_id,
tt.rowid AS transaction_type_id,
r.rowid AS action_id,
f.price AS amount, 
NEW.datetime AS datetime
FROM reservations r
INNER JOIN transaction_types tt
ON 'reserve' = tt.name
INNER JOIN meal_plans mp
ON NEW.meal_plan_id = mp.rowid
INNER JOIN foods f
ON mp.food_id = f.rowid
WHERE NEW.rowid = r.rowid; 
END;
