-- Keep a log of any SQL queries you execute as you solve the mystery.

-- examine crime_scene_reports table for day and street of interest
SELECT id, description
  FROM crime_scene_reports
 WHERE year = "2021"
   AND month = "7"
   AND day = "28"
   AND street = "Humphrey Street";
-- results: id 295 ; check the bakery


-- Check interviews to see if something interesting comes up
SELECT id, name, transcript
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND transcript LIKE "%bakery%";
   -- within 10 mins after theft, thief left with car
   -- thief withdraw atm earlier that morning
   -- flight day after booked by someone else

-- check bakery logs 10 mins after theft
SELECT id, hour, minute, activity, license_plate
  FROM bakery_security_logs
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute >= 15;
   -- Lot of logs, interesting to check the exit after 10:15 (theft time)
   -- license_plate in (5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 22W7JE, 0NTHK55, 0NTHK55)

-- Check atm withdrawals before Eugene (witness) arrive at bakery, he said the thief withdrawed at atm earlier that morning
SELECT *
  FROM atm_transactions
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND atm_location LIKE "%Leggett Street%";
   -- account number found in withdraws: (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199)

-- check id of these persons, matching withdrawals and exiting bakery logs
SELECT *
  FROM people
 WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199))
   AND license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "22W7JE", "0NTHK55", "0NTHK55");
-- GOT 3 names
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 396669 | Iman  | (829) 555-5269 | 7049073643      | L93JTIZ       |
--| 467400 | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |

-- Whick one of them called that day? less than a minute
SELECT *
  FROM phone_calls
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60
   AND caller IN ("(829) 555-5269", "(389) 555-5198", "(367) 555-5533");
--caller IN ("Iman", "Luca", "Bruce")
-- FOUND Caller and receiver numbers, caller (367) 555-5533, receiver (375) 555-8161 duration 45

-- Check name of the thief and accomplice
SELECT *
  FROM people
  WHERE phone_number IN ("(367) 555-5533", "(375) 555-8161");
  --|   id   | name  |  phone_number  | passport_number | license_plate |
  --+--------+-------+----------------+-----------------+---------------+
  --| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | Thief
  --| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       | Accomplice


-- check earliest flights on day after theft, out of FIFTYVILLE
SELECT * FROM flights
 WHERE origin_airport_id IN (SELECT id FROM airports WHERE city LIKE "Fiftyville")
 AND year = 2021
 AND month = 7
 AND day = 29
 ORDER BY hour
 LIMIT 1;
 -- flight they took
 -- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
 -- +----+-------------------+------------------------+------+-------+-----+------+--------+
 -- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |

--Check if passengers on flight
SELECT * FROM passengers
 WHERE flight_id = 36
   AND passport_number = 5773159633;
   -- seat 4A

-- City where they escaped
SELECT city FROM airports
 WHERE id IN (SELECT destination_airport_id FROM flights WHERE id = 36);
 -- new york city!