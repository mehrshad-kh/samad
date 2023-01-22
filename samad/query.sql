CREATE TRIGGER after_reservation
AFTER INSERT ON reservations
BEGIN
	--  INSERT INTO transactions (user_id, transaction_type_id, action_id, amount, datetime) 
	SELECT NEW.user_id, tt.rowid, NEW.rowid, f.price, NEW.datetime
	FROM reservations r
	INNER JOIN transaction_types tt
	ON 'reserve' = tt.name
	INNER JOIN meal_plans mp
	ON r.meal_plan_id = mp.rowid
	INNER JOIN foods f
	ON mp.food_id foods.rowid;
END;
 
-- CREATE TRIGGER after_reservation2
-- AFTER INSERT ON reservations
-- BEGIN
-- 	INSERT INTO transactions (user_id, datetime)
--	VALUES (NEW.user_id, NEW.datetime);
-- END;

-- INSERT INTO transactions (
--	user_id, 
--	transaction_type_id, 
--	action_id, 
--	amount, 
--	datetime 
-- )
-- SELECT r.user_id AS user_id, 
-- tt.rowid AS transaction_type_id, 
-- r.rowid AS action_id, 
-- f.price AS food_price, 
-- r.datetime AS datetime
-- FROM reservations r
-- INNER JOIN transaction_types tt
-- ON 'reserve' = tt.name
-- INNER JOIN meal_plans mp
-- ON r.meal_plan_id = mp.rowid
-- INNER JOIN foods f
-- ON f.rowid = mp.food_id
-- WHERE r.rowid = 1;
