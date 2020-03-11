package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.*;

@Data
@Table(name = "my_collection")
public class MyCollection implements Serializable {
    @Column(name = "account_id")
    private Long accountId;

    @Column(name = "resource_id")
    private Long resourceId;

    private Integer type;

    private Boolean status;

    @Column(name = "c_time")
    private Date cTime;

    @Column(name = "u_time")
    private Date uTime;

    private static final long serialVersionUID = 1L;


    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(getClass().getSimpleName());
        sb.append(" [");
        sb.append("Hash = ").append(hashCode());
        sb.append(", accountId=").append(accountId);
        sb.append(", resourceId=").append(resourceId);
        sb.append(", status=").append(status);
        sb.append(", cTime=").append(cTime);
        sb.append(", uTime=").append(uTime);
        sb.append(", serialVersionUID=").append(serialVersionUID);
        sb.append("]");
        return sb.toString();
    }
}