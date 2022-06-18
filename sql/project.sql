
CREATE TABLE task (
    id              BIGINT PRIMARY KEY,
    name            VARCHAR(256) UNIQUE,
    rtsp_address    VARCHAR(256) unique,
    algorithms      VARCHAR(256),
    ip              VARCHAR(256),
    port            SMALLINT,
    current_time    VARCHAR(256),
    update_time     VARCHAR(256)
);

CREATE TABLE algo (
    id              BIGINT PRIMARY KEY,
    name            VARCHAR(256) UNIQUE,
    conf            DOUBLE,
    obj             DOUBLE,
    iou             DOUBLE,
    dev_id          INTEGER,
    pos_list        VARCHAR(256),
    var             DOUBLE,
    yd_time         INTEGER,
    lg_time         INTEGER,
    jg_time         INTEGER,
    cz_time         INTEGER,
    person_number   INTEGER
);

CREATE TABLE warning (
    id              BIGINT PRIMARY KEY,
    alarm_code      VARCHAR(256),
    alarm_remark    VARCHAR(256),
    alarm_time      VARCHAR(256),
    alarm_video     VARCHAR(256),
    alarm_warning_img   VARCHAR(256),
    alarm_img       VARCHAR(256)
);