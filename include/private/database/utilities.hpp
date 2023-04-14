#ifndef PRIVATE_DATABASE_UTILITIES_HPP
#define PRIVATE_DATABASE_UTILITIES_HPP
#include <ostream>
#include <soci/soci.h>
namespace
{
class BigInt
{
public:
    BigInt() = default;
    BigInt(const int64_t i) : mI(i) {}

    void set(const int64_t i) { mI = i; }
    int64_t get() const { return mI; }

private:
    int64_t mI{0};
};

[[maybe_unused]]
std::ostream& operator<<(std::ostream &os, const BigInt &bigInt)
{
    os << bigInt.get();
    return os;
}

[[maybe_unused]]
void createTable(soci::session &session)
{
    const std::string stationData = R"""(
CREATE TABLE IF NOT EXISTS station_data (
 identifier INTEGER PRIMARY KEY NOT NULL,
 network VARCHAR(32) NOT NULL,
 station VARCHAR(32) NOT NULL,
 latitude DOUBLE PRECISION NOT NULL CHECK(latitude >= -90 AND latitude <= 90),
 longitude DOUBLE PRECISION NOT NULL, 
 elevation DOUBLE PRECISION,
 ondate DOUBLE NOT NULL,
 offdate DOUBLE NOT NULL CHECK(ondate < offdate),
 description VARCHAR(256) DEFAULT '',
 lddate TIMESTAMP CURRENT_TIMESTAMP
 );
)""";
    const std::string channelData = R"""(
CREATE TABLE IF NOT EXISTS channel_data (
 identifier INTEGER PRIMARY KEY NOT NULL,
 network VARCHAR(32) NOT NULL,
 station VARCHAR(32) NOT NULL,
 channel VARCHAR(32) NOT NULL,
 location_code VARCHAR(32) NOT NULL DEFAULT '01',
 latitude DOUBLE PRECISION NOT NULL CHECK(latitude >= -90 AND latitude <= 90),
 longitude DOUBLE PRECISION NOT NULL, 
 elevation DOUBLE PRECISION,
 sampling_rate DOUBLE PRECISION CHECK(sampling_rate > 0),
 azimuth DOUBLE PRECISION CHECK(azimuth >= 0 AND azimuth < 360),
 dip DOUBLE PRECISION CHECK(dip >= -90 AND dip <= 90),
 ondate DOUBLE NOT NULL,
 offdate DOUBLE NOT NULL CHECK(ondate < offdate),
 lddate TIMESTAMP CURRENT_TIMESTAMP
);
)""";
    const std::string magnitude = R"""(
CREATE TABLE IF NOT EXISTS magnitude (
 identifier INTEGER PRIMARY KEY NOT NULL,
 magnitude DOUBLE PRECISION NOT NULL CHECK(magnitude > -11 AND magnitude < 11),
 magnitude_type VARCHAR(8) NOT NULL,
 lddate TIMESTAMP CURRENT_TIMESTAMP
);
)""";
    const std::string origin = R"""(
CREATE TABLE IF NOT EXISTS origin (
 identifier INTEGER PRIMARY KEY NOT NULL,
 latitude DOUBLE PRECISION NOT NULL CHECK(latitude >= -90 AND latitude < 90),
 longitude DOUBLE PRECISION NOT NULL,
 depth DOUBLE PRECISION NOT NULL CHECK(depth > -10 AND depth < 1000),
 time DOUBLE NOT NULL,
 authority VARCHAR(128) DEFAULT 'UU',
 lddate TIMESTAMP CURRENT_TIMESTAMP
 );
)""";
    const std::string arrival = R"""(
CREATE TABLE IF NOT EXISTS arrival (
 identifier INTEGER PRIMARY KEY NOT NULL,
 origin INTEGER NOT NULL,
 network VARCHAR(32) NOT NULL,
 station VARCHAR(32) NOT NULL,
 channel VARCHAR(32) NOT NULL,
 location_code VARCHAR(32) NOT NULL DEFAULT '01',
 time DOUBLE NOT NULL,
 phase VARCHAR(8) NOT NULL,
 first_motion INTEGER DEFAULT 0,
 review_status VARCHAR(1) DEFAULT 'A',
 lddate TIMESTAMP CURRENT_TIMESTAMP,
 FOREIGN KEY(origin) REFERENCES origin(identifier)
);
)""";
    const std::string waveform = R"""(
CREATE TABLE IF NOT EXISTS waveform (
 identifier INTEGER PRIMARY KEY NOT NULL,
 network VARCHAR(32) NOT NULL,
 station VARCHAR(32) NOT NULL,
 channel VARCHAR(32) NOT NULL,
 location_code VARCHAR(32) NOT NULL DEFAULT '01',
 event_identifier INTEGER,
 ontime DOUBLE NOT NULL,
 offtime DOUBLE NOT NULL CHECK(ontime < offtime),
 filename TEXT NOT NULL,
 lddate TIMESTAMP CURRENT_TIMESTAMP,
 FOREIGN KEY(event_identifier) REFERENCES event(identifier)
 );
)""";
    const std::string event = R"""(
CREATE TABLE IF NOT EXISTS event (
 identifier INTEGER PRIMARY KEY NOT NULL,
 preferred_origin INTEGER NOT NULL,
 preferred_magnitude INTEGER NOT NULL,
 event_type VARCHAR(2) NOT NULL DEFAULT 'uk',
 review_status VARCHAR(8) NOT NULL DEFAULT 'A',
 lddate TIMESTAMP CURRENT_TIMESTAMP,
 FOREIGN KEY(preferred_origin) REFERENCES origin(identifier),
 FOREIGN KEY(preferred_magnitude) REFERENCES magnitude(identifier)
 );
)""";
    session << stationData;
    session << channelData;
    session << magnitude;
    session << origin;
    session << arrival;
    session << waveform;
    session << event;
}

[[maybe_unused]]
void deleteTable(soci::session &session)
{
    session << "DELETE FROM station_data;";
    session << "DELETE FROM channel_data;";
    session << "DELETE FROM magnitude;";
    session << "DELETE FROM origin;";
    session << "DELETE FROM arrival;";
    session << "DELETE FROM waveform;";
    session << "DELETE FROM event;";
}

[[maybe_unused]]
void dropTable(soci::session &session)
{
    session << "DROP TABLE station_data;";
    session << "DROP TABLE channel_data;";
    session << "DROP TABLE magnitude;";
    session << "DROP TABLE origin;";
    session << "DROP TABLE arrival;";
    session << "DROP TABLE waveform;";
    session << "DROP TABLE event;";
}

}

/*
namespace soci
{
    template <> struct type_conversion<BigInt>
    {   
        typedef long long base_type;
        static void from_base(long long i, indicator ind, BigInt &mi)
        {
            if (ind == i_null)
            {
                throw soci_error("Null value not allowed for this type");
            }
            mi.set(i);
        }
        static void to_base(const BigInt & mi, long long &i, indicator & ind)
        {
            i = mi.get();
            ind = i_ok;
        }
    };  
}
*/
#endif
