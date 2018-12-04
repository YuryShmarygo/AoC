
-- task 1 step1: guard 283
SELECT guard, Sum(sleep_time) FROM (
SELECT e2.name guard, Round((e.event_ts - e3.event_ts) * 24 * 60) sleep_time
 FROM ys_events e, ys_events e2, ys_events e3
WHERE e.name='wakes up'
 AND e2.name LIKE 'Guard%' AND e2.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name LIKE 'Guard%' AND event_ts < e.event_ts AND event_ts > e2.event_ts)
 AND e3.name='falls asleep' AND e3.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name='falls asleep' AND event_ts < e.event_ts AND event_ts > e3.event_ts)
) GROUP BY guard
ORDER BY 2 desc;

-- task 1 step2: guard 283 * 43
WITH ys_events2 AS (
SELECT e2.name guard, Round((e.event_ts - e3.event_ts) * 24 * 60) sleep_time, e3.event_ts - Trunc(e3.event_ts) fall_asleep, e.event_ts - Trunc(e.event_ts) wakes_up
 FROM ys_events e, ys_events e2, ys_events e3
WHERE e.name='wakes up'
 AND e2.name LIKE 'Guard%' AND e2.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name LIKE 'Guard%' AND event_ts < e.event_ts AND event_ts > e2.event_ts)
 AND e3.name='falls asleep' AND e3.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name='falls asleep' AND event_ts < e.event_ts AND event_ts > e3.event_ts)
 )
SELECT guard, Round(fall_asleep * 24 * 60), Round(wakes_up*24*60), (SELECT Count(*) FROM ys_events2 WHERE guard = e.guard AND fall_asleep <= e.fall_asleep AND wakes_up > e.fall_asleep )
FROM ys_events2 e WHERE guard = 'Guard #283 begins shift'
ORDER BY 4 desc;


-- task 2 guard 449 * 36
WITH ys_events2 AS (
SELECT e2.name guard, Round((e.event_ts - e3.event_ts) * 24 * 60) sleep_time, e3.event_ts - Trunc(e3.event_ts) fall_asleep, e.event_ts - Trunc(e.event_ts) wakes_up
 FROM ys_events e, ys_events e2, ys_events e3
WHERE e.name='wakes up'
 AND e2.name LIKE 'Guard%' AND e2.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name LIKE 'Guard%' AND event_ts < e.event_ts AND event_ts > e2.event_ts)
 AND e3.name='falls asleep' AND e3.event_ts < e.event_ts AND NOT EXISTS(SELECT 1 FROM ys_events WHERE name='falls asleep' AND event_ts < e.event_ts AND event_ts > e3.event_ts)
 )
SELECT guard, Round(fall_asleep * 24 * 60), Round(wakes_up*24*60), (SELECT Count(*) FROM ys_events2 WHERE guard = e.guard AND fall_asleep <= e.fall_asleep AND wakes_up > e.fall_asleep )
FROM ys_events2 e
ORDER BY 4 desc;

