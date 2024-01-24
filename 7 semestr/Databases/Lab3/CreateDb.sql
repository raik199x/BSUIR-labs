-- Dropping everything
DROP TABLE IF EXISTS reservation;
DROP TABLE IF EXISTS film_actor;
DROP TABLE IF EXISTS cinema_hall;
DROP TABLE IF EXISTS employee;
DROP TABLE IF EXISTS job_title;
DROP TABLE IF EXISTS actors;
DROP TABLE IF EXISTS film;
DROP TABLE IF EXISTS client;
-- Creating basic tables
CREATE TABLE job_title (title text PRIMARY KEY, duties text);
CREATE TABLE employee (
	full_name text PRIMARY KEY,
	job_title text references job_title(title),
	salary int,
	start_time time,
	end_time time
);
CREATE TABLE cinema_hall (
	hall_name text PRIMARY KEY,
	assigned_employee text references employee(full_name),
	hall_number int,
	total_number_of_seats int
);
CREATE TABLE actors (
	full_name text PRIMARY KEY,
	rating decimal,
	age int
);
CREATE TABLE film (
	film_name text PRIMARY KEY,
	genres text [],
	stars decimal
);
CREATE TABLE client(
	full_name text PRIMARY KEY,
	email text,
	ticket_number serial
);
-- Many to many connections
CREATE TABLE reservation(
	cinema_hall_name text references cinema_hall(hall_name),
	client_full_name text references client(full_name),
	playing_film_name text references film(film_name),
	identification_number serial PRIMARY KEY,
	date_of_play date,
	column_number int,
	sit_number int
);
CREATE TABLE film_actor (
	specific_film_name text,
	actor_name text,
	actor_role text,
	PRIMARY KEY (specific_film_name, actor_name, actor_role),
	FOREIGN KEY (specific_film_name) REFERENCES film (film_name),
	FOREIGN KEY (actor_name) REFERENCES actors (full_name)
);