import { useEffect, useState } from "react";
import { useRouter } from "next/router";

export default function Dashboard() {
  const [data, setData] = useState(null);
  const { query } = useRouter();

  useEffect(() => {
    if (query.apiKey) {
      fetch(`/api/stats?apiKey=${query.apiKey}`)
        .then(res => res.json())
        .then(setData);
    }
  }, [query.apiKey]);

  if (!data) return <p>Loading...</p>;

  return (
    <div>
      <h1>HackaTime Dashboard</h1>
      <p>Total Time Today: {data.totalTimeText}</p>
      <pre>{JSON.stringify(data, null, 2)}</pre>
    </div>
  );
}
