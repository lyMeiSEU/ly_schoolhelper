package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.*;

@Data
public class Favorite implements Serializable {
    @Column(name = "account_id")
    private Long accountId;

    @Column(name = "favorite_account_id")
    private Long favoriteAccountId;

    private Integer status;

    @Column(name = "c_time")
    private Date cTime;

    @Column(name = "u_time")
    private Date uTime;

    private static final long serialVersionUID = 1L;
}