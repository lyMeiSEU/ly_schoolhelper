package cn.edu.seu.alumni_server.dataSync.entity;

import javax.persistence.Column;
import javax.persistence.Table;
import java.io.Serializable;

@Table(name = "Work")

public class Work implements Serializable {
    private String company;

    private String job;

    private String openid;

    private String num;

    @Column(name = "start_year")
    private String startYear;

    @Column(name = "end_year")
    private String endYear;

    private static final long serialVersionUID = 1L;

    /**
     * @return company
     */
    public String getCompany() {
        return company;
    }

    /**
     * @param company
     */
    public void setCompany(String company) {
        this.company = company == null ? null : company.trim();
    }

    /**
     * @return job
     */
    public String getJob() {
        return job;
    }

    /**
     * @param job
     */
    public void setJob(String job) {
        this.job = job == null ? null : job.trim();
    }

    /**
     * @return openid
     */
    public String getOpenid() {
        return openid;
    }

    /**
     * @param openid
     */
    public void setOpenid(String openid) {
        this.openid = openid == null ? null : openid.trim();
    }

    /**
     * @return num
     */
    public String getNum() {
        return num;
    }

    /**
     * @param num
     */
    public void setNum(String num) {
        this.num = num == null ? null : num.trim();
    }

    /**
     * @return start_year
     */
    public String getStartYear() {
        return startYear;
    }

    /**
     * @param startYear
     */
    public void setStartYear(String startYear) {
        this.startYear = startYear == null ? null : startYear.trim();
    }

    /**
     * @return end_year
     */
    public String getEndYear() {
        return endYear;
    }

    /**
     * @param endYear
     */
    public void setEndYear(String endYear) {
        this.endYear = endYear == null ? null : endYear.trim();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(getClass().getSimpleName());
        sb.append(" [");
        sb.append("Hash = ").append(hashCode());
        sb.append(", company=").append(company);
        sb.append(", job=").append(job);
        sb.append(", openid=").append(openid);
        sb.append(", num=").append(num);
        sb.append(", startYear=").append(startYear);
        sb.append(", endYear=").append(endYear);
        sb.append(", serialVersionUID=").append(serialVersionUID);
        sb.append("]");
        return sb.toString();
    }
}