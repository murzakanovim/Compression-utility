CREATE TABLE t_event
(
    id        SERIAL PRIMARY KEY,
    type      VARCHAR,
    subjects  VARCHAR,
    timestamp VARCHAR,
    zip_event VARCHAR,
    ts_vector VARCHAR
)