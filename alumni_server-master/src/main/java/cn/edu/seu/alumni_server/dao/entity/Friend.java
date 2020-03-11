package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.*;

/**
 * 数据库里的朋友是这样维护的
 * 每个朋友关系有两条记录，因为两个人的关系可能不同（如拉黑/拒绝等情况下,
 * 或者类似A关注B,B未关注A这种，两个人关系并不严格对等的情况），也方便查询
 * 不然两个人关系正反打，关系状态就 * 2 了，而且查询sql也长了一倍，正反查
 * 但是这样就要求，关系改动时，要同时修改两个人的关系，通过事务保持一致
 *
 */
@Data
public class Friend implements Serializable {
    @Column(name = "account_id")
    private Long accountId;

    @Column(name = "friend_account_id")
    private Long friendAccountId;

    private Integer status;

    @Column(name = "c_time", insertable = false)
    private Date cTime;

    @Column(name = "u_time", insertable = false)
    private Date uTime;

    @Column(name = "valid_status", insertable = false)
    private Boolean validStatus;

    private static final long serialVersionUID = 1L;
}