import { useState, useEffect } from "react";

const SOURCES = ["Cook", "Sabato", "270toWin", "Inside Elections", "Ballotpedia"];
const RATING_ORDER = ["Safe D", "Likely D", "Lean D", "Tilt D", "Toss-up", "Tilt R", "Lean R", "Likely R", "Safe R"];
const CONSENSUS_ORDER = ["Safe D", "Likely D", "Lean D", "Toss-up", "Lean R", "Likely R", "Safe R"];
const LAST_UPDATED = "May 30, 2026";

const INITIAL_CHANGES = {
  ch: [
    "Refreshed: May 30, 2026",
    "NJ-07 (Kean, R): Note updated — Kean has not cast a House vote since March 5 due to undisclosed health issue. Gave first interview May 21 saying he will return 'in next few weeks.' IE moved from Tilt R to Toss-up May 21. Ratings unchanged.",
    "PA-10 (Perry, R): IE moved from Tilt R to Toss-up May 21. Ratings already reflected correctly.",
    "No other Competitive House rating changes since May 27.",
  ],
  cs: [
    "Refreshed: May 30, 2026",
    "GA Senate: CORRECTED — Cook rates Toss-up (updated May 20); Sabato moved to Lean D in January 2026 after Gov. Kemp declined to run. GOP heads to June 16 runoff between Rep. Mike Collins and Derek Dooley. Cook column corrected from Lean D to Toss-up.",
    "TX Senate: CORRECTED — Sabato column updated from Likely R to Lean R. Both Cook and Sabato now at Lean R after Paxton defeated Cornyn in May 26 runoff. New poll (May 27-28) shows Talarico leading 47-44%. Sabato would need Talarico in 'higher 40s' consistently before moving to Toss-up.",
    "IA Senate: MAJOR CHANGE — Ernst not seeking re-election. Cook moved Lean R to Toss-up. Dem primary June 2: Turek vs. Wahls.",
  ],
  lh: [
    "Refreshed: May 30, 2026",
    "VA-07 (Vindman, D): REMOVED — Inside Elections moved Lean D to Likely D on May 21.",
    "TX-35 (Open/Casar, D retiring): ADDED — Cook moved Likely R to Lean R on May 27. Open seat in Austin/San Antonio area.",
    "No other Leaning House rating changes since May 27.",
  ],
};

const PANEL_KEYS = { "Competitive House": "ch", "Competitive Senate": "cs", "Leaning House": "lh" };

function withConsensus(races) {
  return races.map(r => {
    const vals = [r.cook, r.sabato, r.t270, r.ie, r.bp].filter(x => x && x !== "N/A");
    const mapped = vals.map(v => {
      if (v === "Tilt D") return CONSENSUS_ORDER.indexOf("Toss-up") - 0.5;
      if (v === "Tilt R") return CONSENSUS_ORDER.indexOf("Toss-up") + 0.5;
      const i = CONSENSUS_ORDER.indexOf(v);
      return i >= 0 ? i : null;
    }).filter(i => i !== null);
    const avg = mapped.length ? mapped.reduce((s, v) => s + v, 0) / mapped.length : null;
    return { ...r, consensus: avg !== null ? CONSENSUS_ORDER[Math.round(avg)] : "N/A" };
  });
}

function ratingStyle(r) {
  if (!r || r === "N/A") return { background: "var(--color-background-secondary)", color: "var(--color-text-secondary)" };
  if (r === "Safe D" || r === "Likely D") return { background: "#dbeafe", color: "#1e3a8a" };
  if (r === "Lean D") return { background: "#bfdbfe", color: "#1e40af" };
  if (r === "Tilt D") return { background: "#e0f2fe", color: "#075985" };
  if (r === "Toss-up") return { background: "#fef9c3", color: "#854d0e" };
  if (r === "Tilt R") return { background: "#ffe4e6", color: "#9f1239" };
  if (r === "Lean R") return { background: "#fecaca", color: "#991b1b" };
  if (r === "Likely R" || r === "Safe R") return { background: "#fee2e2", color: "#7f1d1d" };
  return { background: "var(--color-background-secondary)", color: "var(--color-text-secondary)" };
}

function Badge({ r }) {
  const s = ratingStyle(r);
  return <span style={{ ...s, borderRadius: 5, padding: "2px 7px", fontSize: 11, fontWeight: 500, whiteSpace: "nowrap", display: "inline-block" }}>{r || "N/A"}</span>;
}

const SOURCE_KEYS = { Cook:"cook", Sabato:"sabato", "270toWin":"t270", "Inside Elections":"ie", Ballotpedia:"bp" };

const HOUSE_COMPETITIVE = withConsensus([
  { district:"AZ-01", incumbent:"Open (Schweikert, R retiring)",  cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Open seat after Schweikert retirement. District shifted ~4 pts R in 2024. Top Dem target." },
  { district:"AZ-06", incumbent:"Juan Ciscomani (R)",             cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"One of the most evenly divided districts nationally. Trump won by <1 pt in 2024." },
  { district:"CA-22", incumbent:"David Valadao (R)",              cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Tilt R",  bp:"Toss-up", note:"District redrawn for 2026. Valadao has repeatedly survived tough cycles. (1)" },
  { district:"CO-08", incumbent:"Gabe Evans (R)",                 cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Freshman R flipped Dem-held seat in 2024. Suburban/exurban Denver; one of few R-held seats Harris won." },
  { district:"FL-25", incumbent:"Debbie Wasserman Schultz (D)",   cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"N/A",     note:"FL Republican gerrymander shifted district from Harris +5 to Trump +9. Cook confirmed Toss-up May 8. Subject to court challenge. (5)" },
  { district:"IA-01", incumbent:"Mariannette Miller-Meeks (R)",   cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Narrowest R House win in 2024 -- won by 799 votes (0.2%). Dem Bohannan running again." },
  { district:"IA-03", incumbent:"Zach Nunn (R)",                  cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Suburban Des Moines district won by Nunn 51.8% in 2024. IE moved from Tilt R to Toss-up May 21." },
  { district:"MI-07", incumbent:"Tom Barrett (R)",                cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Freshman R won open Lansing-area seat in 2024. Trump won narrowly." },
  { district:"NE-02", incumbent:"Open (Bacon, R retiring)",       cook:"Toss-up", sabato:"Lean D",  t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Open seat; Harris won by ~4.6 pts in 2024. Sabato moved to Lean D Apr. 21. Only NE district allocating its Electoral Vote independently. (4)" },
  { district:"NJ-07", incumbent:"Thomas Kean Jr. (R)",            cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Suburban NJ; Kean won by ~4 pts in 2024. Kean has not cast a House vote since March 5 due to undisclosed health issue; gave first interview May 21 saying he will return 'in next few weeks.' IE moved from Tilt R to Toss-up May 21." },
  { district:"NY-17", incumbent:"Mike Lawler (R)",                cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Tilt R",  bp:"Toss-up", note:"One of only 3 R incumbents in Harris-won districts. Lawler mulling governor run. (2)" },
  { district:"OH-01", incumbent:"Greg Landsman (D)",              cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Cook upgraded from Lean D to Toss-up. Cincinnati-area district Trump carried by ~3 pts in 2024." },
  { district:"OH-09", incumbent:"Marcy Kaptur (D)",               cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Longest-serving woman in Congress. Trump won district by 11 pts. Top GOP pickup opportunity." },
  { district:"PA-07", incumbent:"Ryan Mackenzie (R)",             cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Lehigh Valley true swing seat. Mackenzie flipped from Dem Wild by ~4,000 votes in 2024." },
  { district:"PA-08", incumbent:"Rob Bresnahan (R)",              cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Moved Lean R to Toss-up Apr. 7. Bresnahan faces scrutiny over 600+ stock trades. (3)" },
  { district:"PA-10", incumbent:"Scott Perry (R)",                cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Former Freedom Caucus chair won by just 1 pt in 2024. IE moved from Tilt R to Toss-up May 21. Dem Stelson running again." },
  { district:"TX-34", incumbent:"Vicente Gonzalez (D)",           cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"South Texas majority-Latino district redrawn by R legislature. Trump won by 10 pts in 2024." },
  { district:"VA-02", incumbent:"Jen Kiggans (R)",                cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Reverted to Toss-up May 8 after VA Supreme Court struck down Democratic gerrymander." },
  { district:"WA-03", incumbent:"Marie Perez (D)",                cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Perez narrowly flipped this SW Washington seat in 2024. Trump won district." },
  { district:"WI-03", incumbent:"Derrick Van Orden (R)",          cook:"Toss-up", sabato:"Tilt R",  t270:"Toss-up", ie:"Toss-up", bp:"Toss-up", note:"Western WI district. Van Orden won by ~4 pts in 2024." },
]);

const SENATE_COMPETITIVE = withConsensus([
  { district:"GA",           incumbent:"Jon Ossoff (D)",          cook:"Toss-up", sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"ME",           incumbent:"Susan Collins (R)",       cook:"Toss-up", sabato:"Toss-up", t270:"Toss-up", ie:"Tilt R",  bp:"Toss-up" },
  { district:"MI",           incumbent:"Open (D retiring)",       cook:"Toss-up", sabato:"Tilt D",  t270:"Toss-up", ie:"Toss-up", bp:"Toss-up" },
  { district:"NC",           incumbent:"Open (R retiring)",       cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"NH",           incumbent:"Open (D retiring)",       cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"OH (special)", incumbent:"Jon Husted (R)",          cook:"Toss-up", sabato:"Tilt R",  t270:"Toss-up", ie:"Toss-up", bp:"Toss-up" },
  { district:"IA",           incumbent:"Open (Ernst, R retiring)",cook:"Toss-up", sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"AK",           incumbent:"Dan Sullivan (R)",        cook:"Lean R",  sabato:"Likely R",t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"TX",           incumbent:"Ken Paxton (R nominee)",  cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
]);

const HOUSE_LEANING = withConsensus([
  { district:"CA-13", incumbent:"Adam Gray (D)",                  cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"CA-45", incumbent:"Derek Tran (D)",                 cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"CA-48", incumbent:"Open (Issa, R retiring)",        cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"FL-14", incumbent:"Kathy Castor (D)",               cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"N/A"     },
  { district:"FL-22", incumbent:"Lois Frankel (D)",               cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"N/A"     },
  { district:"MI-04", incumbent:"Open (R, Leans R)",              cook:"N/A",     sabato:"Lean R",  t270:"N/A",     ie:"N/A",     bp:"N/A"     },
  { district:"MI-10", incumbent:"Open (James, R running for gov)",cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"NM-02", incumbent:"Gabe Vasquez (D)",               cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"NV-03", incumbent:"Susie Lee (D)",                  cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"NC-01", incumbent:"Don Davis (D)",                  cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"NC-11", incumbent:"Chuck Edwards (R)",              cook:"N/A",     sabato:"Lean R",  t270:"N/A",     ie:"N/A",     bp:"N/A"     },
  { district:"NY-03", incumbent:"Tom Suozzi (D)",                 cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"NY-04", incumbent:"Laura Gillen (D)",               cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"NY-19", incumbent:"Josh Riley (D)",                 cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"OH-13", incumbent:"Emilia Sykes (D)",               cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"PA-01", incumbent:"Brian Fitzpatrick (R)",          cook:"Likely R",sabato:"Likely R",t270:"Likely R",ie:"Lean R",  bp:"Likely R" },
  { district:"TX-28", incumbent:"Henry Cuellar (D)",              cook:"Lean D",  sabato:"Lean D",  t270:"Lean D",  ie:"Lean D",  bp:"Lean D"  },
  { district:"TX-35", incumbent:"Open (Casar, D retiring)",       cook:"Lean R",  sabato:"N/A",     t270:"N/A",     ie:"N/A",     bp:"N/A"     },
  { district:"VA-01", incumbent:"Rob Wittman (R)",                cook:"Lean R",  sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"AK-AL", incumbent:"Nick Begich (R)",                cook:"Likely R",sabato:"Lean R",  t270:"Lean R",  ie:"Lean R",  bp:"Lean R"  },
  { district:"ME-02", incumbent:"Open (Golden, D retiring)",      cook:"Likely R",sabato:"Likely R",t270:"Likely R",ie:"Lean R",  bp:"Likely R" },
  { district:"WI-03", incumbent:"Derrick Van Orden (R)",          cook:"Toss-up", sabato:"Tilt R",  t270:"Toss-up", ie:"Toss-up", bp:"Toss-up" },
]);

const DATA = {
  "Competitive House": HOUSE_COMPETITIVE,
  "Competitive Senate": SENATE_COMPETITIVE,
  "Leaning House": HOUSE_LEANING,
};

const PANEL_LABELS = {
  "Competitive House": "Competitive House races",
  "Competitive Senate": "Competitive Senate races",
  "Leaning House": "Leaning House seats",
};

// Simple cleared state — no storage dependency for showing changes
function RecentChanges({ panel }) {
  const [cleared, setCleared] = useState(false);
  const changes = INITIAL_CHANGES[PANEL_KEYS[panel]] || [];

  return (
    <div style={{ border: "2px solid #000", borderRadius: "var(--border-radius-lg)", padding: "12px 16px", marginBottom: "1.25rem", background: "var(--color-background-secondary)" }}>
      <div style={{ display: "flex", justifyContent: "space-between", alignItems: "center", marginBottom: cleared ? 6 : (changes.length ? 10 : 0) }}>
        <span style={{ fontSize: 13, fontWeight: 500 }}>Recent changes</span>
        {!cleared && changes.length > 0 && (
          <button onClick={() => setCleared(true)} style={{ fontSize: 11, padding: "3px 10px", cursor: "pointer", color: "var(--color-text-secondary)" }}>
            Clear
          </button>
        )}
      </div>
      {cleared ? (
        <p style={{ fontSize: 12, color: "var(--color-text-secondary)", margin: 0 }}>Cleared. Changes will appear here after the next refresh.</p>
      ) : changes.length === 0 ? (
        <p style={{ fontSize: 12, color: "var(--color-text-secondary)", margin: 0 }}>No changes recorded yet.</p>
      ) : (
        <ul style={{ margin: 0, padding: "0 0 0 1rem", fontSize: 12, color: "var(--color-text-secondary)" }}>
          {changes.map((c, i) => <li key={i} style={{ marginBottom: 4 }}>{c}</li>)}
        </ul>
      )}
    </div>
  );
}

export default function App() {
  const [active, setActive] = useState("Competitive House");
  const [sort, setSort] = useState({ col: "district", dir: 1 });

  function handleSort(col) {
    setSort(s => ({ col, dir: s.col === col ? -s.dir : 1 }));
  }

  const rows = [...DATA[active]].sort((a, b) => {
    const colKey = SOURCE_KEYS[sort.col] || sort.col;
    if (colKey === "district") return sort.dir * a.district.localeCompare(b.district);
    if (colKey === "incumbent") return sort.dir * a.incumbent.localeCompare(b.incumbent);
    const order = colKey === "consensus" ? CONSENSUS_ORDER : RATING_ORDER;
    const va = order.indexOf(a[colKey] || "N/A");
    const vb = order.indexOf(b[colKey] || "N/A");
    return sort.dir * ((va === -1 ? 99 : va) - (vb === -1 ? 99 : vb));
  });

  const thStyle = col => ({
    padding: "7px 10px", textAlign: "left", fontWeight: 500,
    color: sort.col === col ? "var(--color-text-primary)" : "var(--color-text-secondary)",
    fontSize: 11, whiteSpace: "nowrap", cursor: "pointer",
    borderBottom: "0.5px solid var(--color-border-tertiary)", userSelect: "none",
  });

  const cols = ["district", "incumbent", "consensus", ...SOURCES, ...(active === "Competitive House" ? ["notes"] : [])];

  return (
    <div style={{ padding: "1.25rem 1rem", maxWidth: 1200, margin: "0 auto" }}>
      <div style={{ marginBottom: "1.25rem" }}>
        <h1 style={{ fontSize: 26, fontWeight: 500, margin: 0 }}>2026 Congressional Race Tracker</h1>
        <p style={{ margin: "4px 0 0", fontSize: 12, color: "var(--color-text-secondary)" }}>
          Data as of {LAST_UPDATED} · Sources: Cook · Sabato · 270toWin · Inside Elections · Ballotpedia
        </p>
      </div>

      <div style={{ display: "flex", gap: 6, marginBottom: "1.25rem", flexWrap: "wrap" }}>
        {Object.keys(PANEL_LABELS).map(p => (
          <button key={p} onClick={() => setActive(p)} style={{
            padding: "5px 14px", cursor: "pointer", fontSize: 13,
            fontWeight: active === p ? 500 : 400,
            borderColor: active === p ? "#166534" : "var(--color-border-tertiary)",
            background: active === p ? "#dcfce7" : "transparent",
            color: active === p ? "#166534" : "var(--color-text-primary)",
          }}>
            {PANEL_LABELS[p]} <span style={{ fontSize: 11, color: active === p ? "#166534" : "var(--color-text-secondary)" }}>({DATA[p].length})</span>
          </button>
        ))}
      </div>

      <RecentChanges panel={active} />

      <div style={{ background: "var(--color-background-primary)", border: "0.5px solid var(--color-border-tertiary)", borderRadius: "var(--border-radius-lg)", overflowX: "auto" }}>
        <table style={{ width: "100%", borderCollapse: "collapse", fontSize: 12 }}>
          <thead>
            <tr>
              {cols.map(col => (
                <th key={col} style={thStyle(col)} onClick={() => handleSort(col)}>
                  {col === "district" ? "District/State" : col === "incumbent" ? "Incumbent" : col === "consensus" ? "Consensus" : col === "notes" ? "Notes (Cook unless noted)" : col}
                  {sort.col === col ? (sort.dir === 1 ? " ↑" : " ↓") : ""}
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            {rows.map((r, i) => (
              <tr key={i} style={{ borderBottom: "0.5px solid var(--color-border-tertiary)" }}>
                <td style={{ padding: "7px 10px", fontWeight: 500, whiteSpace: "nowrap" }}>{r.district}</td>
                <td style={{ padding: "7px 10px", color: "var(--color-text-secondary)", whiteSpace: "nowrap", fontSize: 11 }}>{r.incumbent}</td>
                <td style={{ padding: "7px 10px" }}><Badge r={r.consensus} /></td>
                {SOURCES.map(s => <td key={s} style={{ padding: "7px 10px" }}><Badge r={r[SOURCE_KEYS[s]]} /></td>)}
                {active === "Competitive House" && <td style={{ padding: "7px 10px", fontSize: 11, color: "var(--color-text-secondary)", minWidth: 260, maxWidth: 360 }}>{r.note || "—"}</td>}
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      <div style={{ marginTop: 10, display: "flex", gap: 6, flexWrap: "wrap" }}>
        {[
          { r:"Lean D",   label:"Lean D" },
          { r:"Tilt D",   label:"Tilt D = slight D edge" },
          { r:"Toss-up",  label:"Toss-up" },
          { r:"Tilt R",   label:"Tilt R = slight R edge" },
          { r:"Lean R",   label:"Lean R" },
          { r:"Likely R", label:"Likely R" },
        ].map(({ r, label }) => <Badge key={r} r={label} />)}
      </div>

      <p style={{ fontSize: 11, color: "var(--color-text-secondary)", marginTop: 8 }}>
        Click any column header to sort. Consensus smooths Tilt ratings into nearest standard tier. Ask me to refresh data at any time.
        {active === "Competitive House" && (
          <span> (1) CA-22: IE re redistricting. (2) NY-17: governor speculation. (3) PA-08: stock trade reporting. (4) NE-02: Sabato Lean D per Apr. 21 update. (5) FL-25: FL Republican gerrymander; still being litigated.</span>
        )}
        <br />Sources:{" "}
        {[
          { name: "Cook Political Report", url: "https://www.cookpolitical.com/ratings/house-race-ratings" },
          { name: "Sabato's Crystal Ball",  url: "https://centerforpolitics.org/crystalball/2026-house/" },
          { name: "270toWin",               url: "https://www.270towin.com/2026-house-election-predictions/" },
          { name: "Inside Elections",       url: "https://www.insideelections.com/ratings/house" },
          { name: "Ballotpedia",            url: "https://ballotpedia.org/United_States_House_of_Representatives_elections,_2026" },
        ].map((s, i, arr) => (
          <span key={s.name}>
            <a href={s.url} style={{ color: "var(--color-text-info)" }}>{s.name}</a>{i < arr.length - 1 ? " · " : ""}
          </span>
        ))}
      </p>
    </div>
  );
}
