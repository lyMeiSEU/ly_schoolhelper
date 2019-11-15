--odps sql 
--********************************************************************--
--author:梅洛瑜
--create time:2019-11-06 10:39:05
--********************************************************************--


DROP TABLE IF EXISTS temp_yiding_data_prepare_for_luoyu;
CREATE TABLE temp_yiding_data_prepare_for_luoyu AS 
Select t1.rider_id_hash, t1.tracking_id_hash, t1.shop_id_hash, t1.user_latitude, t1.user_longitude,
t1.shop_latitude, t1.shop_longitude, t1.restaurant_accept_order_time, t1.rider_accept_order_time, 
t1.rider_pickup_time, t1.rider_arrive_restaurant_time, t1.rider_delivery_time, t2.sku_id_hash
From(
    SELECT
    order_id,
    sha2(rider_id, 256) as rider_id_hash,
    sha2(tracking_id, 256) as tracking_id_hash,
    sha2(shop_id, 256) as shop_id_hash,
    user_latitude,
    user_longitude,
    shop_latitude,
    shop_longitude,
    restaurant_confirm_time as restaurant_accept_order_time,
    rider_accept_order_time,
    rider_pickup_time,
    rider_arrive_restaurant_time,
    rider_delivery_time,
    ds
FROM
    tracking_data_table
WHERE
    (
        (
            ds >= '20180924'
            AND ds <= '20181007'
        )
        OR(ds = '20180917')
    )
    AND(
        sha2(shop_id, 256) = '174a34ea86e2372a4976071e55559993eae1aec1b6332c7b3e77451c4d72303c'
        OR sha2(shop_id, 256) = ' 8c39c56bb341610574c94cf6820a7a339a364c69eb3ee52946139251eeb2b6d1'
        OR sha2(shop_id, 256) = '8fc1c2681c89453380b3044435b3274f6e4810069d5a0e1d614e0bc4a0b647b5'
    )

)t1
join (
    Select order_id, sha2(sku_id, 256) as sku_id_hash,ds
    From sku_data_table
    WHERE (
        (
            ds >= '20180924'
            AND ds <= '20181007'
        )
        OR(ds = '20180917')
    )
    AND(
        sha2(shop_id, 256) = '174a34ea86e2372a4976071e55559993eae1aec1b6332c7b3e77451c4d72303c'
        OR sha2(shop_id, 256) = ' 8c39c56bb341610574c94cf6820a7a339a364c69eb3ee52946139251eeb2b6d1'
        OR sha2(shop_id, 256) = '8fc1c2681c89453380b3044435b3274f6e4810069d5a0e1d614e0bc4a0b647b5'
    )
)t2
on
t1.ds=t2.ds
AND t1.order_id=t2.order_id;

