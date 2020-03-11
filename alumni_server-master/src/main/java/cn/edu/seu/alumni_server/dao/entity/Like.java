package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Table;
import java.io.Serializable;
import java.sql.Date;

@Data
@Table(name="like")
public class Like implements Serializable {

    private Long accountId;

    private Long resourceId;

    private Integer type;

    private Boolean status;

    private Date cTime;

    private Date uTime;

    private static final long serialVersionUID = 1L;
}
