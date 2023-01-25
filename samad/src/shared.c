//
//  shared.c
//  samad
//
//  Created by Mehrshad on 10/10/1401 AP.
//

#include "shared.h"

extern const char *const kErr;
extern const char *const kAllocationErr;
extern const char *const kQueryGenerationErr;
extern const char *const kQueryExecutionErr;
extern const char *const kDatabaseOpenErr;
extern const char *const kDatabaseCloseErr;

sqlite3 *OpenDatabase(const char *filename)
{
    int rc = 0;
    
    sqlite3 *db = NULL;
    
    rc = sqlite3_open(filename, &db);
    
    if (rc == SQLITE_OK) {
        return db;
    } else {
        fprintf(stderr, "%s %s: %s\n",
                kErr, kDatabaseOpenErr, sqlite3_errmsg(db));
        return NULL;
    }
}

void CloseDatabase(sqlite3 *db)
{
    int rc = 0;
    
    rc = sqlite3_close(db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n",
               kErr, kDatabaseCloseErr, sqlite3_errmsg(db));
    }
}

int CreateUsersTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY, user_type INTEGER, activated BOOLEAN, "
    "first_name VARCHAR(100), last_name VARCHAR(100), "
    "id_number VARCHAR(50) UNIQUE, national_id VARCHAR(50), "
    "birthdate TEXT, "
    "sex TINYINT CHECK (sex = 0 OR sex = 1 OR sex = 2 OR sex = 9), "
    "balance INTEGER CHECK (balance >= 0), "
    "password VARCHAR(200), salt BLOB, "
    "FOREIGN KEY (user_type) REFERENCES user_types (id) "
    ");";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateLunchroomsTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS lunchrooms ("
    "id INTEGER PRIMARY KEY, name VARCHAR(100), address VARCHAR(300), "
    "capacity INTEGER, sex TINYINT CHECK (sex=0 OR sex=1 OR sex=2 OR sex=9));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateFoodsTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS foods ("
    "id INTEGER PRIMARY KEY, name VARCHAR(100), "
    "food_type VARCHAR(100), price INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateMealTypesTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS meal_types ("
    "id INTEGER PRIMARY KEY, name VARCHAR(200), "
    "start_time TEXT, end_time TEXT);";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }
    
    return value;
}

int CreateLunchroomMealTypesTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS lunchroom_meal_types ("
    "id INTEGER PRIMARY KEY, lunchroom_id INTEGER, meal_type_id INTEGER, "
    "FOREIGN KEY (lunchroom_id) REFERENCES lunchrooms (id), "
    "FOREIGN KEY (meal_type_id) REFERENCES meal_types (id) "
    ");";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }
    
    return value;
}

int CreateMealPlansTable(sqlite3 *db)
{
    int rc = 0;
    int value = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS meal_plans ("
    "id INTEGER PRIMARY KEY, food_id INTEGER, "
    "lunchroom_id INTEGER, meal_type_id INTEGER, "
    "food_quantity INTEGER, date TEXT, "
    "FOREIGN KEY (food_id) REFERENCES foods (id), "
    "FOREIGN KEY (lunchroom_id) REFERENCES lunchrooms (id), "
    "FOREIGN KEY (meal_type_id) REFERENCES meal_types (id) "
    ");";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc == SQLITE_OK) {
        value = 0;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        value = -1;
    }

    return value;
}

int CreateReservationsTable(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS reservations ("
    "id INTEGER PRIMARY KEY, user_id INTEGER, "
    "meal_plan_id INTEGER, datetime TEXT, "
    "FOREIGN KEY (user_id) REFERENCES users (id), "
    "FOREIGN KEY (meal_plan_id) REFERENCES meal_plans (id) "
    ");";
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
    }
    
    return rc;
}

int ExecuteQuery(sqlite3 *db, const char *sql)
{
    int rc = 0;
    char *err_msg = NULL;
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
    }
    
    return rc;
}

int CreateTables(sqlite3 *db)
{
    int rc = 0;
    
    rc = ExecuteQuery(db, "PRAGMA foreign_keys = ON;");
    if (rc != 0)
        goto exit;
    
    rc = ExecuteQuery(db, "CREATE TABLE IF NOT EXISTS user_types ("
                      "id INTEGER PRIMARY KEY, name VARCHAR(100) UNIQUE);");
    if (rc != 0)
        goto exit;
    rc = ExecuteQuery(db, "INSERT OR IGNORE INTO user_types (name)"
                      "VALUES ('employee'), ('student');");
    if (rc != 0)
        goto exit;
    
    rc = CreateUsersTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateLunchroomsTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateMealTypesTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateLunchroomMealTypesTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateFoodsTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateMealPlansTable(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateReservationsTable(db);
    if (rc != 0)
        goto exit;
    
    rc = ExecuteQuery(db, "CREATE TABLE IF NOT EXISTS transaction_types ("
                      "id INTEGER PRIMARY KEY, name VARCHAR(100) UNIQUE);");
    if (rc != 0)
        goto exit;
    rc = ExecuteQuery(db, "INSERT OR IGNORE INTO transaction_types (name) "
                      "VALUES ('reserve'), ('charge');");
    if (rc != 0)
        goto exit;
    
    rc = ExecuteQuery(db, "CREATE TABLE IF NOT EXISTS transactions ("
                      "id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "transaction_type_id INTEGER, "
                      "action_id INTEGER, "
                      "amount INTEGER, "
                      "datetime TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users (id), "
                      "FOREIGN KEY (transaction_type_id) "
                      "REFERENCES transaction_types (id) "
                      ");");
    if (rc != 0)
        goto exit;
    
exit:
    return rc;
}

int CreateTriggers(sqlite3 *db)
{
    int rc = 0;
    
    // rc = ExecuteQuery(db, "CREATE TRIGGER IF NOT EXISTS after_reserve "
    //                   "AFTER INSERT ON reservations "
    //                   "BEGIN "
    //                   "INSERT INTO transactions ("
    //                   "user_id, transaction_type_id, "
    //                   "action_id, amount, datetime) "
    //                   "SELECT NEW.user_id AS user_id, "
    //                   "tt.id AS transaction_type_id, "
    //                   "NEW.id AS action_id, "
    //                   "-f.price AS amount, "
    //                   "NEW.datetime AS datetime "
    //                   "FROM reservations r "
    //                   "INNER JOIN transaction_types tt "
    //                   "ON 'reserve' = tt.name "
    //                   "INNER JOIN meal_plans mp "
    //                   "ON NEW.meal_plan_id = mp.id "
    //                   "INNER JOIN foods f "
    //                   "ON mp.food_id = f.id "
    //                   "WHERE NEW.id = r.id;"
    //                   "END;");
    // if (rc != 0)
    //     goto exit;
    
    rc = ExecuteQuery(db, "CREATE TRIGGER IF NOT EXISTS after_balance_update "
                      "AFTER UPDATE ON users "
                      "BEGIN "
                      "INSERT INTO transactions ( "
                      "user_id, transaction_type_id, "
                      "action_id, amount, datetime) "
                      "SELECT NEW.id AS user_id, "
                      "tt.id AS transaction_type_id, "
                      "0 AS action_id, "
                      "(NEW.balance - OLD.balance) AS amount, "
                      "datetime('now', 'localtime') AS datetime "
                      "FROM users u "
                      "INNER JOIN transaction_types tt "
                      "ON 'charge' = tt.name "
                      "WHERE u.id = NEW.id; "
                      "END;");
    if (rc != 0)
        goto exit;
    
exit:
    return rc;
}

int InitDatabase(sqlite3 *db)
{
    int rc = 0;
    
    rc = CreateTables(db);
    if (rc != 0)
        goto exit;
    
    rc = CreateTriggers(db);
    if (rc != 0)
        goto exit;
    
exit:
    return rc;
}

bool IsFirstLaunch(sqlite3 *db)
{
    bool value = false;
    int rc = 0;
    int user_count = 0;
    char *err_msg = NULL;
    char *sql = "SELECT COUNT(*) FROM users;";

    rc = sqlite3_exec(db, sql, &CountCallback, &user_count, &err_msg);

    if (rc == SQLITE_OK) {
        if (user_count == 0)
            value = true;
        else
            value = false;
    } else {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
    }

    return value;
}

void PerformAccountCreation(sqlite3 *db, int user_type)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;

    int activated = (user_type == kStudent ? 0 : 1);
    char *first_name = NULL;
    char *last_name = NULL;
    char *id_number = NULL;
    char *national_id = NULL;
    char *birthdate = NULL;
    int sex = 0;
    char *password = NULL;

    printf("\n--ACCOUNT CREATION--\n");
    printf("Please complete the following form.\n");

    if (user_type == kOptional) {
        printf("Account type (%d: employee, %d: student): ",
               kEmployee, kStudent);
        user_type = TakeIntInput();
    }

    printf("First name: ");
    TakeStringInput(&first_name);

    printf("Last name: ");
    TakeStringInput(&last_name);

    printf("ID number: ");
    TakeStringInput(&id_number);

    printf("National ID: ");
    TakeStringInput(&national_id);

    printf("Birthdate (YYYY-MM-DD): ");
    TakeStringInput(&birthdate);

    printf("Sex (1: male, 2: female): ");
    sex = TakeIntInput();

    password = getpass("Password: ");

    if ((user_type != kEmployee && user_type != kStudent)
        || (sex != kMale && sex != kFemale)) {
        printf("Invalid input.\nPlease try again later.\n");
        goto exit2;
    } else if (!IsDateCorrect(birthdate)) {
        printf("Invalid date.\nDid you enter in Persian calendar?\n");
        goto exit2;
    }
    
    rc = asprintf(&sql, "INSERT INTO users ("
                  "user_type, activated, first_name, last_name, "
                  "id_number, national_id, birthdate, sex, balance, "
                  "password, salt) "
                  "VALUES (%d, %d, '%s', '%s', '%s', "
                  "'%s', '%s', %d, %d, '%s', %d);",
                  user_type, activated, first_name, last_name, id_number,
                  national_id, birthdate, sex, 0, password, 0);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit;
    }
    
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    printf("The account was successfully created.\n");
exit:
    free(sql);
exit2:
    free(first_name);
    free(last_name);
    free(id_number);
    free(national_id);
    free(birthdate);
}

struct User *PerformLogin(sqlite3 *db)
{
    int rc = 0;
    char *err_msg = NULL;
    char *sql = NULL;
    
    char *username = NULL;
    char *password = NULL;
    // char *password = "1234";
    struct User *user = NULL;
    
    printf("\n--LOGIN--\n");
    
    printf("Username: ");
    TakeStringInput(&username);
    
    password = getpass("Password: ");

    rc = asprintf(&sql, "SELECT * FROM users "
                  "WHERE id_number = '%s' "
                  "AND password = '%s';",
                  username, password);
    if (rc == -1) {
        fprintf(stderr, "%s %s\n", kErr, kQueryGenerationErr);
        goto exit1;
    }
    
    rc = sqlite3_exec(db, sql, &LoginCallback, &user, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "%s %s: %s\n", kErr, kQueryExecutionErr, err_msg);
        sqlite3_free(err_msg);
        goto exit;
    }
    
    if (user == NULL) {
        printf("Your username or password is incorrect.\n"
               "Please try again later.\n");
    } else if (user->activated == 0) {
        printf("Your account is not activated.\n"
               "Please contact the administrator.\n");
        user = NULL;
    }
    else {
        printf("Logged in as %s %s\n", user->first_name, user->last_name);
    }
    
exit:
    free(sql);
exit1:
    free(username);
    
    return user;
}

void PerformLogout(struct User **user)
{
    free((*user)->first_name);
    free((*user)->last_name);
    free((*user)->id_number);
    free((*user)->national_id);
    free((*user)->birthdate);
    free(*user);
    
    *user = NULL;
}

void DisplayLoginMenu(sqlite3 *db)
{
    int input = 0;
    int is_first_launch = 0;
    int user_type = 0;
    struct User *user = NULL;
    
    is_first_launch = IsFirstLaunch(db);
    
    printf("\n--SAMAD--\n");
    printf("0: Quit\n"
           "1: Log in\n"
           "2: Sign up\n");

input_generation:
    input = TakeShellInput();

    switch (input) {
        case 0:
            printf("Have a great day!\n");
            goto exit;
            break;
        case 1:
            if (is_first_launch) {
                printf("No admins on file.\n");
                goto input_generation;
            } else {
                user = PerformLogin(db);
                if (user == NULL) {
                    DisplayLoginMenu(db);
                } else {
                    if (user->user_type == kEmployee) {
                        DisplayAdminMenu(db, &user);
                    } else {
                        DisplayStudentMenu(db, &user);
                    }
                }
            }
            break;
        case 2:
            user_type = is_first_launch ? kEmployee : kStudent;
            PerformAccountCreation(db, user_type);
            DisplayLoginMenu(db);
            break;
        default:
            printf("Invalid input. Please try again.\n");
            goto input_generation;
    }
    
exit:
    input = 0;
}
